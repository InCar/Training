#include "stdafx.h"
#include "resource.h"
#include "MainWnd12.h"

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
	wcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
	ComPtr<ID3D12Debug3> ptrDebug;
	D3D12GetDebugInterface(IID_PPV_ARGS(&ptrDebug));
	ptrDebug->EnableDebugLayer();

	uFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
	// DXGI
	ComPtr<IDXGIFactory7> ptrDXGIFactory;
	HRESULT hr = CreateDXGIFactory2(uFlags, IID_PPV_ARGS(&ptrDXGIFactory));
	if (FAILED(hr)) return FALSE;
	
	// 创建D3D设备对象
	hr = CreateDevice(ptrDXGIFactory);
	if (FAILED(hr)) return FALSE;

	// 创建双缓冲交换链
	hr = CreateSwapChain(ptrDXGIFactory, rc.right, rc.bottom);
	if (FAILED(hr)) return FALSE;

	// 创建资源堆
	hr = CreateD3D12Heaps();
	if (FAILED(hr)) return FALSE;

	// Init Box
	hr = m_box.CreateD3DRes(m_ptrDev, m_ptrCmdList);
	if (FAILED(hr)) return FALSE;

	// 执行
	hr = m_ptrCmdList->Close();
	ID3D12CommandList* pCmdList = m_ptrCmdList.Get();
	m_ptrCmdQueue->ExecuteCommandLists(1, &pCmdList);
	if (FAILED(hr)) return FALSE;

	// Resize
	hr = Resize(rc.right, rc.bottom);
	if (FAILED(hr)) return FALSE;

	return TRUE;
}

HRESULT MainWnd12::CreateDevice(ComPtr<IDXGIFactory7> &ptrDXGIFactory)
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

HRESULT MainWnd12::CreateSwapChain(ComPtr<IDXGIFactory7> &ptrDXGIFactory, int cx, int cy)
{
	DXGI_SWAP_CHAIN_DESC1 desc{
		(UINT)cx, // width
		(UINT)cy, // height
		c_fmt,
		false, //stereo
		{1, 0}, // MSAA
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

HRESULT MainWnd12::Resize(int cx, int cy)
{
	// 让先前命令队列中的命令都执行完
	FlushCmdQueue();

	// 重置命令链表
	HRESULT hr = m_ptrCmdList->Reset(m_ptrCmdAllocator.Get(), nullptr);

	// 释放先前创建的资源
	for (int i = 0; i < c_uBufferCount; i++) {
		m_ptrBackBuf[i] = nullptr;
	}
	m_ptrDepthBuf = nullptr;

	// 重设双缓冲尺寸
	hr = m_ptrDXGISwapChain->ResizeBuffers(c_uBufferCount, cx, cy, c_fmt, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	m_uBackBufIdx = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE hHeapCPU = m_ptrRTVHeap->GetCPUDescriptorHandleForHeapStart();
	UINT szRTV = m_ptrDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	// 创建渲染视图
	for (int i = 0; i < c_uBufferCount; i++) {
		hr = m_ptrDXGISwapChain->GetBuffer(i, IID_PPV_ARGS(&m_ptrBackBuf[i]));
		m_ptrDev->CreateRenderTargetView(m_ptrBackBuf[i].Get(), nullptr, hHeapCPU);
		hHeapCPU.ptr += szRTV;
	}

	// 创建深度缓冲区
	hr = CreateDepthBuffer(cx, cy);

	// SetViewPort
	SetViewPort(cx, cy);

	// Execute
	hr = m_ptrCmdList->Close();
	ID3D12CommandList *pCmdList =  m_ptrCmdList.Get();
	m_ptrCmdQueue->ExecuteCommandLists(1, &pCmdList);
	FlushCmdQueue();

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
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		{1, 0}, // MSAA
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	};

	D3D12_CLEAR_VALUE clear{
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		{1.0f, 0}
	};

	HRESULT hr = m_ptrDev->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &descBuf, D3D12_RESOURCE_STATE_COMMON, &clear, IID_PPV_ARGS(&m_ptrDepthBuf));

	D3D12_DEPTH_STENCIL_VIEW_DESC descDSV{
		DXGI_FORMAT_D24_UNORM_S8_UINT,
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

void MainWnd12::SetViewPort(int cx, int cy)
{
	D3D12_VIEWPORT viewPort{
		0.0f, 0.0f,
		static_cast<float>(cx), static_cast<float>(cy),
		0.0f, 1.0f
	};

	m_ptrCmdList->RSSetViewports(1, &viewPort);
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
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	EndPaint(hwnd, &ps);
}

BOOL MainWnd12::OnEraseBkgnd(HWND hwnd, HDC hdc)
{
	return TRUE;
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