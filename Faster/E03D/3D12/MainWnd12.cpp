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

LRESULT MainWnd12::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 消息处理
	switch (uMsg) {
		HANDLE_MSG(hWnd, WM_ERASEBKGND, OnEraseBkgnd);
		break;
	default:
		return XWnd::WndProc(hWnd, uMsg, wParam, lParam);
	}
}

BOOL MainWnd12::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	__super::OnCreate(hwnd, lpCreateStruct);

	HRESULT hr;
#ifdef _DEBUG
	// 输出更多调试信息
	ComPtr<ID3D12Debug> spDebug;
	hr = D3D12GetDebugInterface(IID_PPV_ARGS(&spDebug));
	if(SUCCEEDED(hr))
		spDebug->EnableDebugLayer();
#endif
	// DXGI
	ComPtr<IDXGIFactory5> spDxgiFactory;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(&spDxgiFactory));
	if (FAILED(hr)) return FALSE;
	
	// 创建D3D物理设备对象
	hr = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_ptrDev));
	if (FAILED(hr)) {
		// 创建失败,替代创建WARP
		ComPtr<IDXGIAdapter> spAdapter;
		hr = spDxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&spAdapter));
		if (FAILED(hr)) return FALSE;
		hr = D3D12CreateDevice(spAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_ptrDev));
		if (FAILED(hr)) return FALSE;
	}


	return TRUE;
}

void MainWnd12::OnDestroy(HWND hwnd)
{
	__super::OnDestroy(hwnd);

	PostQuitMessage(0);
}

void MainWnd12::OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	EndPaint(hwnd, &ps);
}

BOOL MainWnd12::OnEraseBkgnd(HWND hwnd, HDC hdc)
{
	return FALSE;
}