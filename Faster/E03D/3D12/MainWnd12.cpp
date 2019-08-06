#include "stdafx.h"
#include "resource.h"
#include "MainWnd12.h"

const DXGI_SAMPLE_DESC MainWnd12::c_mass = { 1, 0 };

MainWnd12::MainWnd12()
{
}

MainWnd12::~MainWnd12()
{
}

ATOM MainWnd12::Register()
{
	// 主窗口类型注册,隐藏了基类方法
	WNDCLASS wcls;
	ZeroMemory(&wcls, sizeof(WNDCLASS));
	wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcls.lpfnWndProc = XWnd::WindowProc;
	wcls.hInstance = m_hInst;
	wcls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcls.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON_APP)); // 图标
	wcls.lpszClassName = m_wstrClsName.c_str();

	return RegisterClass(&wcls);
}

BOOL MainWnd12::Create()
{
	// 窗口尺寸和位置
	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYSCREEN);
	int width = cxScreen > 800 ? 800 : cxScreen;
	int height = cyScreen > 600 ? 600 : cyScreen;

	WNDCLASS cls;
	if (!GetClassInfo(m_hInst, m_wstrClsName.c_str(), &cls)) Register();

	// 创建窗口
	wstring wstrTitle(L"3D12");
	m_hwnd = CreateWindow(m_wstrClsName.c_str(), wstrTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		(cxScreen - width) / 2, (cyScreen - height) / 2, width, height,
		NULL, NULL, m_hInst, this);
	if (!m_hwnd) return FALSE;

	// 第一次刷新窗口
	UpdateWindow(m_hwnd);

	return TRUE;
}

LRESULT MainWnd12::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 消息处理
	switch (uMsg) {
		HANDLE_MSG(hwnd, WM_ERASEBKGND, OnEraseBkgnd);
		HANDLE_MSG(hwnd, WM_SIZE, OnSize);
		break;
	default:
		return XWnd::WndProc(hwnd, uMsg, wParam, lParam);
	}
}

BOOL MainWnd12::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	__super::OnCreate(hwnd, lpCreateStruct);

	RECT rc;
	GetClientRect(hwnd, &rc);

	// 启用调试信息
	UINT uFlags = 0;
#ifdef _DEBUG
	ComPtr<ID3D12Debug> ptrDebug;
	D3D12GetDebugInterface(IID_PPV_ARGS(&ptrDebug));
	ptrDebug->EnableDebugLayer();

	uFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
	// DXGI
	ComPtr<IDXGIFactory4> ptrDXGIFactory;
	HRESULT hr = CreateDXGIFactory2(uFlags, IID_PPV_ARGS(&ptrDXGIFactory));
	if (FAILED(hr)) return FALSE;
	
	// 创建D3D设备对象 以及 相关联的命令队列
	hr = CreateDevice(ptrDXGIFactory);
	if (FAILED(hr)) return FALSE;

	// 初始化几何对象
	hr = m_box.CreateD3DRes(m_ptrCmdList);
	if (FAILED(hr)) return FALSE;

	// 创建资源描述符堆
	hr = CreateD3D12Heaps();
	if (FAILED(hr)) return FALSE;
	
	// 3D空间
	hr = Set3DSpace();
	if (FAILED(hr)) return FALSE;

	// RootSignature
	hr = CreateRootSignature();
	if (FAILED(hr)) return FALSE;
	
	// GPU代码
	hr = LoadShaders();
	if (FAILED(hr)) return FALSE;

	// 流水线
	hr = BuildPSO();
	if (FAILED(hr)) return FALSE;

	// 创建后备缓冲交换链
	hr = CreateSwapChain(ptrDXGIFactory, rc.right, rc.bottom);
	if (FAILED(hr)) return FALSE;

	// 执行
	hr = m_ptrCmdList->Close();
	ID3D12CommandList* pCmdList = m_ptrCmdList.Get();
	m_ptrCmdQueue->ExecuteCommandLists(1, &pCmdList);
	if (FAILED(hr)) return FALSE;

	// Resize
	hr = Resize(rc.right, rc.bottom);
	if (FAILED(hr)) return FALSE;

	// 记录时间戳
	QueryPerformanceFrequency(reinterpret_cast<PLARGE_INTEGER>(&m_i64Freq));
	QueryPerformanceCounter(reinterpret_cast<PLARGE_INTEGER>(&m_i64Start));

	return TRUE;
}

void MainWnd12::OnDestroy(HWND hwnd)
{
	__super::OnDestroy(hwnd);

	PostQuitMessage(0);
}

void MainWnd12::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	Resize(cx, cy);
}

void MainWnd12::OnPaint(HWND hwnd)
{
	// 更新ConstantBuffer
	Update();

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	EndPaint(hwnd, &ps);

	// 渲染
	Render();

	// 如果窗口尺寸不为0,不停绘制
	RECT rc;
	GetClientRect(hwnd, &rc);
	if (rc.right > 0 && rc.bottom > 0) 
		InvalidateRect(hwnd, nullptr, false);
}

BOOL MainWnd12::OnEraseBkgnd(HWND hwnd, HDC hdc)
{
	return TRUE;
}

HRESULT MainWnd12::CreateDevice(ComPtr<IDXGIFactory4> &ptrDXGIFactory)
{
	HRESULT hr = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_ptrDev));
	if (FAILED(hr)) {
		OutputDebugString(L"The D3D12 is not supported by the hardware.\n");
		// 创建硬件加速设备失败,替代创建WARP设备
		ComPtr<IDXGIAdapter> spAdapter;
		hr = ptrDXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&spAdapter));
		if (FAILED(hr)) return hr;
		hr = D3D12CreateDevice(spAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_ptrDev));
		if (FAILED(hr)) return hr;
		else {
			m_bWARP = true;
			OutputDebugString(L"The WARP device has been created.\n");
		}
	}

	// 创建同步fence
	hr = m_ptrDev->CreateFence(m_u64Fence, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_ptrFence));
	if (FAILED(hr)) return hr;

	// 创建命令队列
	D3D12_COMMAND_QUEUE_DESC descQueue{
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
		D3D12_COMMAND_QUEUE_FLAG_NONE,
		0
	};
	hr = m_ptrDev->CreateCommandQueue(&descQueue, IID_PPV_ARGS(&m_ptrCmdQueue));
	if (FAILED(hr)) return hr;

	// 创建命令链表
	hr = m_ptrDev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_ptrCmdAllocator));
	if (FAILED(hr)) return hr;

	hr = m_ptrDev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_ptrCmdAllocator.Get(), nullptr, IID_PPV_ARGS(&m_ptrCmdList));
	if (FAILED(hr)) return hr;

	return hr;
}

HRESULT MainWnd12::CreateSwapChain(ComPtr<IDXGIFactory4> &ptrDXGIFactory, int cx, int cy)
{
	DXGI_SWAP_CHAIN_DESC1 desc{
		(UINT)cx, // width
		(UINT)cy, // height
		c_fmtBack,
		false, //stereo
		c_mass, // MSAA
		DXGI_USAGE_RENDER_TARGET_OUTPUT,
		c_uBufferCount, // buffer count
		DXGI_SCALING_NONE,
		DXGI_SWAP_EFFECT_FLIP_DISCARD,
		DXGI_ALPHA_MODE_UNSPECIFIED,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	};

	HRESULT hr = ptrDXGIFactory->CreateSwapChainForHwnd(m_ptrCmdQueue.Get(), m_hwnd, &desc, NULL, NULL, &m_ptrDXGISwapChain);
	return hr;
}

HRESULT MainWnd12::CreateD3D12Heaps()
{
	// RTV Heap
	D3D12_DESCRIPTOR_HEAP_DESC descRTV{
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		c_uBufferCount,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};
	HRESULT hr = m_ptrDev->CreateDescriptorHeap(&descRTV, IID_PPV_ARGS(&m_ptrRTVHeap));
	if (FAILED(hr)) return hr;

	// DSV Heap
	D3D12_DESCRIPTOR_HEAP_DESC descDSV{
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};
	hr = m_ptrDev->CreateDescriptorHeap(&descDSV, IID_PPV_ARGS(&m_ptrDSVHeap));

	// CBV Heap
	D3D12_DESCRIPTOR_HEAP_DESC descCBV{
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		0
	};
	hr = m_ptrDev->CreateDescriptorHeap(&descCBV, IID_PPV_ARGS(&m_ptrCBVHeap));

	return hr;
}

HRESULT MainWnd12::Set3DSpace()
{
	// 必须是256的整数倍
	unsigned int uLeft = sizeof(WorldViewProj) & 0x00ff;
	unsigned int uPadding = 0x0100 - uLeft;
	unsigned int szCBV = sizeof(WorldViewProj) + uPadding;

	D3D12_HEAP_PROPERTIES heap{
		D3D12_HEAP_TYPE_UPLOAD,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0
	};

	D3D12_RESOURCE_DESC desc{
		D3D12_RESOURCE_DIMENSION_BUFFER,
		0, // alignment
		szCBV,
		1,
		1,
		1,
		DXGI_FORMAT_UNKNOWN,
		{ 1, 0 },
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		D3D12_RESOURCE_FLAG_NONE
	};
	// m_ptrCB
	HRESULT hr = m_ptrDev->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_ptrCB));

	// CBView
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = m_ptrCB->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = szCBV;
	D3D12_CPU_DESCRIPTOR_HANDLE hHeapCPU = m_ptrCBVHeap->GetCPUDescriptorHandleForHeapStart();
	m_ptrDev->CreateConstantBufferView(&cbvDesc, hHeapCPU);

	// 标准空间
	m_wvProj.mWorld = ::XMMatrixIdentity();

	// 摄像机位
	XMVECTOR Eye = XMVectorSet(2.0f, 3.0f, -8.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_wvProj.mView = ::XMMatrixLookAtLH(Eye, At, Up);

	return hr;
}

HRESULT MainWnd12::CreateRootSignature()
{
	HRESULT hr;

	// register(b0)
	D3D12_DESCRIPTOR_RANGE range{
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
		1,
		0,
		0,
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
	};

	D3D12_ROOT_PARAMETER param{
		D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
		{
			1,
			&range
		},
		D3D12_SHADER_VISIBILITY_ALL
	};

	D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc{
		D3D_ROOT_SIGNATURE_VERSION_1,
		{
			1,
			&param,
			0,
			nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
		}
	};

	ComPtr<ID3DBlob> ptrBlob, ptrError;
	hr = D3D12SerializeVersionedRootSignature(&desc, &ptrBlob, &ptrError);
	hr = m_ptrDev->CreateRootSignature(0, ptrBlob->GetBufferPointer(), ptrBlob->GetBufferSize(), IID_PPV_ARGS(&m_ptrRootSig));
	return hr;
}

HRESULT MainWnd12::LoadShaders()
{
	HRESULT hr = LoadShader(L"VS12.cso", m_ptrVSBlob);
	if (FAILED(hr)) return hr;

	hr = LoadShader(L"PS12.cso", m_ptrPSBlob);
	return hr;
}

HRESULT MainWnd12::LoadShader(const wchar_t* shaderFileName, ComPtr<ID3DBlob>& ptrBlob)
{
	// 相对路径
	wchar_t buf[512];
	GetModuleFileName(NULL, buf, 512);
	wchar_t* pLast = wcsrchr(buf, L'\\');
	if (pLast == NULL) return 0;

	// read from file
	wcscpy_s(pLast + 1, 512 - ((pLast + 1) - buf), shaderFileName);
	HANDLE hCSO = CreateFile(buf, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hCSO == INVALID_HANDLE_VALUE) return E_HANDLE;
	DWORD dwSize = GetFileSize(hCSO, NULL);

	HRESULT hr = D3DCreateBlob(dwSize, ptrBlob.GetAddressOf());
	ReadFile(hCSO, ptrBlob->GetBufferPointer(), dwSize, &dwSize, NULL);
	CloseHandle(hCSO);

	return hr;
}

HRESULT MainWnd12::BuildPSO()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
	memset(&desc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	// root signature
	desc.pRootSignature = m_ptrRootSig.Get();
	// shaders
	desc.VS = { m_ptrVSBlob->GetBufferPointer(), m_ptrVSBlob->GetBufferSize() };
	desc.PS = { m_ptrPSBlob->GetBufferPointer(), m_ptrPSBlob->GetBufferSize() };
	// blend state
	for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		auto &target = desc.BlendState.RenderTarget[i];
		target.SrcBlend      = D3D12_BLEND_ONE,      target.DestBlend = D3D12_BLEND_ZERO,      target.BlendOp = D3D12_BLEND_OP_ADD;
		target.SrcBlendAlpha = D3D12_BLEND_ONE, target.DestBlendAlpha = D3D12_BLEND_ZERO, target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		target.LogicOp = D3D12_LOGIC_OP_NOOP;
		target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	}
	desc.SampleMask = 0xFFFFFFFF;
	// rasterizer state
	desc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	desc.RasterizerState.DepthClipEnable = true;
	// depth stencil
	desc.DepthStencilState.DepthEnable = true;
	desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	desc.DepthStencilState.StencilEnable = false;
	desc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	desc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	desc.DepthStencilState.FrontFace = { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
	desc.DepthStencilState.BackFace = desc.DepthStencilState.FrontFace;
	// input layout
	auto &vInputs = m_box.GetInputDesc();
	desc.InputLayout = { vInputs.data(), static_cast<UINT>(vInputs.size()) };
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// render target
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = c_fmtBack;
	desc.DSVFormat = c_fmtDepthStencil;
	desc.SampleDesc = c_mass; // MSAA

#ifdef _DEBUG
	// WARP device only
	if(m_bWARP) desc.Flags = D3D12_PIPELINE_STATE_FLAG_TOOL_DEBUG;
#endif

	HRESULT hr = m_ptrDev->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_ptrPSO));
	return hr;
}

HRESULT MainWnd12::Resize(int cx, int cy)
{
	// 尺寸不能是0
	if (cx == 0) cx = 1;
	if (cy == 0) cy = 1;

	// 让先前命令队列中的命令都执行完
	FlushCmdQueue();

	// 重置命令链表
	HRESULT hr = m_ptrCmdList->Reset(m_ptrCmdAllocator.Get(), nullptr);
	if (FAILED(hr)) return hr;

	// 释放先前创建的资源
	for (int i = 0; i < c_uBufferCount; i++) {
		m_ptrBackBuf[i] = nullptr;
	}
	m_ptrDepthBuf = nullptr;

	// 重设后备缓冲区尺寸
	hr = ResizeBackBuffer(cx, cy);
	if (FAILED(hr)) return hr;

	// 创建深度缓冲区
	hr = CreateDepthBuffer(cx, cy);
	if (FAILED(hr)) return hr;

	// 计算视口&裁剪尺寸
	m_viewPort = {
		0.0f, 0.0f,
		static_cast<float>(cx), static_cast<float>(cy),
		0.0f, 1.0f
	};
	m_rectScissor = {
		0, 0,
		cx, cy
	};

	// 根据新的纵横比重设投影
	m_wvProj.mProj = ::XMMatrixPerspectiveFovLH(XM_PI / 6.0f, cx / (FLOAT)(cy), 0.01f, 100.0f);

	// Execute
	hr = m_ptrCmdList->Close();
	ID3D12CommandList *pCmdList =  m_ptrCmdList.Get();
	m_ptrCmdQueue->ExecuteCommandLists(1, &pCmdList);
	FlushCmdQueue();

	return hr;
}

HRESULT MainWnd12::ResizeBackBuffer(int cx, int cy)
{
	// 重设后备缓冲尺寸
	HRESULT hr = m_ptrDXGISwapChain->ResizeBuffers(c_uBufferCount, cx, cy, c_fmtBack, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	D3D12_CPU_DESCRIPTOR_HANDLE hHeapCPU = m_ptrRTVHeap->GetCPUDescriptorHandleForHeapStart();
	UINT szRTV = m_ptrDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// 创建渲染视图
	for (int i = 0; i < c_uBufferCount; i++) {
		hr = m_ptrDXGISwapChain->GetBuffer(i, IID_PPV_ARGS(&m_ptrBackBuf[i]));
		m_ptrDev->CreateRenderTargetView(m_ptrBackBuf[i].Get(), nullptr, hHeapCPU);
		hHeapCPU.ptr += szRTV;
	}

	// 重置当前缓冲区索引序号
	m_uBackBufIdx = 0;

	return hr;
}

HRESULT MainWnd12::CreateDepthBuffer(int cx, int cy)
{
	D3D12_HEAP_PROPERTIES heap{
		D3D12_HEAP_TYPE_DEFAULT,
		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		D3D12_MEMORY_POOL_UNKNOWN,
		0,
		0
	};

	D3D12_RESOURCE_DESC descBuf{
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		static_cast<UINT64>(cx),
		static_cast<UINT>(cy),
		1,
		1, // mip
		c_fmtDepthStencil,
		c_mass, // MSAA
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	};

	D3D12_CLEAR_VALUE clear{
		c_fmtDepthStencil,
		{1.0f, 0}
	};

	HRESULT hr = m_ptrDev->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &descBuf, D3D12_RESOURCE_STATE_COMMON, &clear, IID_PPV_ARGS(&m_ptrDepthBuf));

	D3D12_DEPTH_STENCIL_VIEW_DESC descDSV{
		c_fmtDepthStencil,
		D3D12_DSV_DIMENSION_TEXTURE2D,
		D3D12_DSV_FLAG_NONE,
		{ 0 }
	};
	D3D12_CPU_DESCRIPTOR_HANDLE hHeapCPU = m_ptrDSVHeap->GetCPUDescriptorHandleForHeapStart();
	m_ptrDev->CreateDepthStencilView(m_ptrDepthBuf.Get(), nullptr, hHeapCPU);

	D3D12_RESOURCE_BARRIER barrier{
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		{
			m_ptrDepthBuf.Get(),
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_DEPTH_WRITE
		}
	};

	m_ptrCmdList->ResourceBarrier(1, &barrier);

	return hr;
}

HRESULT MainWnd12::Update()
{
	// time
	static __int64 s_i64Last = m_i64Start;
	__int64 i64Now;
	QueryPerformanceCounter(reinterpret_cast<PLARGE_INTEGER>(&i64Now));
	float fAngle = XM_PI * (i64Now - m_i64Start) / m_i64Freq;

	// window title
	wchar_t buf[128];
	swprintf_s(buf, 128, L"3D12 - FPS: %7.2f\n", 1.0f * m_i64Freq / (i64Now - s_i64Last));
	SetWindowText(m_hwnd, buf);
	s_i64Last = i64Now;


	// 更新 const buffer
	WorldViewProj constBuf;
	constBuf.mWorld = ::XMMatrixTranspose(m_wvProj.mWorld * ::XMMatrixRotationY(fAngle));
	constBuf.mView = ::XMMatrixTranspose(m_wvProj.mView);
	constBuf.mProj = ::XMMatrixTranspose(m_wvProj.mProj);

	void* pData;
	unsigned uSize = sizeof(WorldViewProj);
	HRESULT hr = m_ptrCB->Map(0, nullptr, &pData);
	memcpy_s(pData, uSize, &constBuf, uSize);
	m_ptrCB->Unmap(0, nullptr);

	return hr;
}

HRESULT MainWnd12::Render()
{
	// 重置命令
	HRESULT hr = m_ptrCmdAllocator->Reset();
	hr = m_ptrCmdList->Reset(m_ptrCmdAllocator.Get(), m_ptrPSO.Get());

	// set view port & scissor
	m_ptrCmdList->RSSetViewports(1, &m_viewPort);
	m_ptrCmdList->RSSetScissorRects(1, &m_rectScissor);

	D3D12_RESOURCE_BARRIER barrierBACK{
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
		D3D12_RESOURCE_BARRIER_FLAG_NONE,
		{
			m_ptrBackBuf[m_uBackBufIdx].Get(),
			D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		}
	};
	m_ptrCmdList->ResourceBarrier(1, &barrierBACK);

	// clear
	auto hHeapRTV = m_ptrRTVHeap->GetCPUDescriptorHandleForHeapStart();
	UINT szRTV = m_ptrDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	hHeapRTV.ptr += szRTV * m_uBackBufIdx;
	m_ptrCmdList->ClearRenderTargetView(hHeapRTV, Colors::Black, 0, nullptr);

	auto hHeadDSV = m_ptrDSVHeap->GetCPUDescriptorHandleForHeapStart();
	m_ptrCmdList->ClearDepthStencilView(hHeadDSV, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Output Merge
	m_ptrCmdList->OMSetRenderTargets(1, &hHeapRTV, true, &hHeadDSV);

	auto pCVBHeap = m_ptrCBVHeap.Get();
	m_ptrCmdList->SetDescriptorHeaps(1, &pCVBHeap);
	m_ptrCmdList->SetGraphicsRootSignature(m_ptrRootSig.Get());
	m_ptrCmdList->SetGraphicsRootDescriptorTable(0, m_ptrCBVHeap->GetGPUDescriptorHandleForHeapStart());

	m_ptrCmdList->IASetVertexBuffers(0, 1, &m_box.GetViewOfVB());
	m_ptrCmdList->IASetIndexBuffer(&m_box.GetViewOfIB());
	m_ptrCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_ptrCmdList->DrawIndexedInstanced(m_box.GetIBCount(), 1, 0, 0, 0);

	barrierBACK.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrierBACK.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	m_ptrCmdList->ResourceBarrier(1, &barrierBACK);

	// 执行
	hr = m_ptrCmdList->Close();
	ID3D12CommandList* pCmd = m_ptrCmdList.Get();
	m_ptrCmdQueue->ExecuteCommandLists(1, &pCmd);

	hr = m_ptrDXGISwapChain->Present(1, 0);
	m_uBackBufIdx = (m_uBackBufIdx + 1) % c_uBufferCount;

	FlushCmdQueue();

	return hr;
}

void MainWnd12::FlushCmdQueue()
{
	m_u64Fence++;
	HRESULT hr = m_ptrCmdQueue->Signal(m_ptrFence.Get(), m_u64Fence);
	if (FAILED(hr)) return;

	if (m_ptrFence->GetCompletedValue() < m_u64Fence) {
		// wait
		HANDLE evCompleted = CreateEvent(nullptr, TRUE, FALSE, nullptr);
		m_ptrFence->SetEventOnCompletion(m_u64Fence, evCompleted);
		WaitForSingleObject(evCompleted, INFINITE);
		CloseHandle(evCompleted);
	}
}