#include "stdafx.h"
#include "resource.h"
#include "MainWnd11.h"

using namespace DirectX;

MainWnd11::MainWnd11()
{
}

MainWnd11::~MainWnd11()
{
}

ATOM MainWnd11::Register()
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

BOOL MainWnd11::Create()
{
	// 窗口尺寸和位置
	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYSCREEN);
	int width = cxScreen > 800 ? 800 : cxScreen;
	int height = cyScreen > 600 ? 600 : cyScreen;

	WNDCLASS cls;
	if (!GetClassInfo(m_hInst, m_wstrClsName.c_str(), &cls)) Register();

	// 创建窗口
	wstring wstrTitle(L"3D11");
	CreateWindow(m_wstrClsName.c_str(), wstrTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		(cxScreen - width) / 2, (cyScreen - height) / 2, width, height,
		NULL, NULL, m_hInst, this);
	if (!m_hwnd) return FALSE;

	// 第一次刷新窗口
	UpdateWindow(m_hwnd);

	return TRUE;
}

LRESULT MainWnd11::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 消息处理
	switch (uMsg) {
		HANDLE_MSG(hwnd, WM_SIZE, OnSize);
		HANDLE_MSG(hwnd, WM_ERASEBKGND, OnEraseBkgnd);
		break;
	default:
		return XWnd::WndProc(hwnd, uMsg, wParam, lParam);
	}
}

BOOL MainWnd11::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	__super::OnCreate(hwnd, lpCreateStruct);

	// DXGI
	ComPtr<IDXGIFactory5> ptrDXGIFactory5;
	HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&ptrDXGIFactory5));
	if (FAILED(hr)) return FALSE;

	// 创建硬件加速的D3D11设备
	hr = CreateDevice(ptrDXGIFactory5);
	if (FAILED(hr)) return FALSE;

	// 创建双缓冲交换链
	hr = CreateSwapChain(ptrDXGIFactory5);
	if (FAILED(hr)) return FALSE;

	// 创建D3D流水线RS状态(Rasterizer Stage)
	hr = CreateRSState();
	if (FAILED(hr)) return FALSE;

	// 加载GPU代码
	hr = LoadShaders();
	if (FAILED(hr)) return FALSE;

	// 设定3D空间
	hr = Set3DSpace();
	if (FAILED(hr)) return FALSE;

	// 创建被绘制对象的资源
	hr = m_box.CreateD3DRes(m_ptrDevice, m_pVS, m_dwVS);
	if (FAILED(hr)) return FALSE;

	return TRUE;
}

HRESULT MainWnd11::CreateDevice(ComPtr<IDXGIFactory5>& ptrDXGIFactory5)
{
	UINT uFlags = 0;

#ifdef _DEBUG
	uFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL level;
	HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, uFlags, nullptr, 0, D3D11_SDK_VERSION, &m_ptrDevice, &level, &m_ptrDevCtx);
	return hr;
}

HRESULT MainWnd11::CreateSwapChain(ComPtr<IDXGIFactory5>& ptrDXGIFactory5)
{
	// 创建双缓冲链
	DXGI_SWAP_CHAIN_DESC descSwapChain{
		   {
			   0,
			   0,
			   { 60, 1 },
			   DXGI_FORMAT_R8G8B8A8_UNORM,
			   DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			   DXGI_MODE_SCALING_UNSPECIFIED
		   },
		   { 4, 0 }, // MSAA 4x
		   DXGI_USAGE_RENDER_TARGET_OUTPUT,
		   1, // double buffers
		   m_hwnd,
		   true, // windowed
		   DXGI_SWAP_EFFECT_DISCARD,
		   0 // DXGI_SWAP_CHAIN_FLAG
	};
	HRESULT hr = ptrDXGIFactory5->CreateSwapChain(m_ptrDevice.Get(), &descSwapChain, &m_ptrDXGISwapChain);
	return hr;
}

HRESULT MainWnd11::CreateDepthBuffer(int cx, int cy)
{
	// 创建Z缓冲区
	D3D11_TEXTURE2D_DESC descDepth{
		   static_cast<UINT>(cx),
		   static_cast<UINT>(cy),
		   1, // mip
		   1,
		   DXGI_FORMAT_D24_UNORM_S8_UINT,
		   { 4, 0 }, // MSAA 4x
		   D3D11_USAGE_DEFAULT,
		   D3D11_BIND_DEPTH_STENCIL,
		   0, // CPU Access
		   0
	};
	HRESULT hr = m_ptrDevice->CreateTexture2D(&descDepth, nullptr, &m_ptrZBuf);
	hr = m_ptrDevice->CreateDepthStencilView(m_ptrZBuf.Get(), nullptr, &m_ptrDSV);
	return hr;
}

HRESULT MainWnd11::CreateRSState()
{
	// 创建D3D流水线RS状态(Rasterizer State)
	D3D11_RASTERIZER_DESC desc;
	memset(&desc, 0, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;
	desc.AntialiasedLineEnable = true;
	HRESULT hr = m_ptrDevice->CreateRasterizerState(&desc, &m_ptrRS);
	return hr;
}

HRESULT MainWnd11::LoadShaders()
{
	m_dwVS = LoadShader(L"VS.cso", &m_pVS);
	HRESULT hr = m_ptrDevice->CreateVertexShader(m_pVS, m_dwVS, NULL, &m_ptrVS);
	if (FAILED(hr)) return hr;

	m_dwPS = LoadShader(L"PS.cso", &m_pPS);
	hr = m_ptrDevice->CreatePixelShader(m_pPS, m_dwPS, NULL, &m_ptrPS);
	return hr;
}

DWORD MainWnd11::LoadShader(const wchar_t* shaderFileName, BYTE** ppBuf)
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
	*ppBuf = new BYTE[dwSize];
	ReadFile(hCSO, *ppBuf, dwSize, &dwSize, NULL);
	CloseHandle(hCSO);

	return dwSize;
}

HRESULT MainWnd11::Set3DSpace()
{
	D3D11_BUFFER_DESC descWorld;
	ZeroMemory(&descWorld, sizeof(D3D11_BUFFER_DESC));
	descWorld.Usage = D3D11_USAGE_DEFAULT;
	descWorld.ByteWidth = sizeof(ConstantBuffer);
	descWorld.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	HRESULT hr = m_ptrDevice->CreateBuffer(&descWorld, NULL, &m_ptrConstant);

	// 标准空间
	m_cb.mWorld = ::XMMatrixIdentity();

	// 摄像机位
	XMVECTOR Eye = XMVectorSet(2.0f, 3.0f, -8.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_cb.mView = ::XMMatrixLookAtLH(Eye, At, Up);

	return hr;
}

void MainWnd11::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	// 重设目标尺寸
	DXGI_MODE_DESC desc{
		(UINT)cx,
		(UINT)cy,
		{60, 1},
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
		DXGI_MODE_SCALING_STRETCHED
	};
	HRESULT hr = m_ptrDXGISwapChain->ResizeTarget(&desc);

	// 尺寸不能是0
	int cx3D = cx == 0 ? 1 : cx;
	int cy3D = cy == 0 ? 1 : cy;

	// 重设双缓冲尺寸
	m_ptrRTV = nullptr;
	m_ptrDSV = nullptr;
	m_ptrZBuf = nullptr;
	hr = m_ptrDXGISwapChain->ResizeBuffers(1, cx3D, cy3D, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	// 创建渲染视图
	ComPtr<ID3D11Texture2D> ptrBackBuf;
	hr = m_ptrDXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&ptrBackBuf));
	hr = m_ptrDevice->CreateRenderTargetView(ptrBackBuf.Get(), nullptr, &m_ptrRTV);

	// 创建Z缓冲
	hr = CreateDepthBuffer(cx3D, cy3D);

	// 设置镜头尺寸(Rasterizer Stage)
	D3D11_VIEWPORT viewPort{
		0.0f, 0.0f,
		static_cast<FLOAT>(cx3D), static_cast<FLOAT>(cy3D),
		0.0f, 1.0f
	};
	m_ptrDevCtx->RSSetViewports(1, &viewPort);

	// 设置D3D流水线OM(Output Merge)
	m_ptrDevCtx->OMSetRenderTargets(1, m_ptrRTV.GetAddressOf(), m_ptrDSV.Get());

	// 设置投影
	m_cb.mProjection = ::XMMatrixPerspectiveFovLH(XM_PI / 6.0f, cx3D / (FLOAT)(cy3D), 0.01f, 100.0f);
}

void MainWnd11::OnDestroy(HWND hwnd)
{
	__super::OnDestroy(hwnd);

	PostQuitMessage(0);
}

void MainWnd11::OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	EndPaint(hwnd, &ps);

	// Clear
	m_ptrDevCtx->ClearRenderTargetView(m_ptrRTV.Get(), Colors::Black);
	m_ptrDevCtx->ClearDepthStencilView(m_ptrDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// 设定D3D流水线IA(Input Assembler)
	m_ptrDevCtx->IASetInputLayout(m_box.GetLayout());
	ID3D11Buffer *pVB = m_box.GetVB();
	UINT stride = m_box.GetVBStride();
	UINT offset = 0;
	m_ptrDevCtx->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);
	m_ptrDevCtx->IASetIndexBuffer(m_box.GetIB(), m_box.GetIBFmt(), 0);
	m_ptrDevCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 设定D3D流水线RS状态(Rasterizer Stage)
	m_ptrDevCtx->RSSetState(m_ptrRS.Get());

	// 设定shaders
	ConstantBuffer cb1;
	cb1.mWorld = XMMatrixTranspose(m_cb.mWorld);
	cb1.mView = XMMatrixTranspose(m_cb.mView);
	cb1.mProjection = XMMatrixTranspose(m_cb.mProjection);
	m_ptrDevCtx->UpdateSubresource(m_ptrConstant.Get(), 0, nullptr, &cb1, 0, 0);
	ID3D11Buffer* pConstant = m_ptrConstant.Get();
	m_ptrDevCtx->VSSetConstantBuffers(0, 1, &pConstant);
	m_ptrDevCtx->VSSetShader(m_ptrVS.Get(), NULL, 0);
	m_ptrDevCtx->PSSetConstantBuffers(0, 1, &pConstant);
	m_ptrDevCtx->PSSetShader(m_ptrPS.Get(), NULL, 0);

	// 在后备缓冲区进行绘制
	m_ptrDevCtx->DrawIndexed(m_box.GetIBCount(), 0, 0);

	// 交换前后缓冲区
	HRESULT hr = m_ptrDXGISwapChain->Present(1, 0);
}

BOOL MainWnd11::OnEraseBkgnd(HWND hwnd, HDC hdc)
{
	// 直接使用D3D清屏,这里直接返回TRUE,不做别的事情
	return TRUE;
}