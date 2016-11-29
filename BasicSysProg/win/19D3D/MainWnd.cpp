#include "stdafx.h"
#include "MainWnd.h"
#include "App.h"

using namespace DirectX;

CMainWnd::CMainWnd()
{
    m_wstrClsName.assign(L"MainWnd");
}


CMainWnd::~CMainWnd()
{
}

ATOM CMainWnd::Register()
{
    HINSTANCE hInstance = CApp::GetApp()->GetInstance();

    WNDCLASS wcls;
    ZeroMemory(&wcls, sizeof(WNDCLASS));
    wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcls.lpfnWndProc = CXWnd::WindowProc;
    wcls.hInstance = hInstance;
    wcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcls.lpszClassName = m_wstrClsName.c_str();
    wcls.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_APP));
    // wcls.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_MAIN);


    return RegisterClass(&wcls);
}

BOOL CMainWnd::Create()
{
    int cxScreen = GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);
    int width = cxScreen > 800 ? 800 : cxScreen;
    int height = cyScreen > 600 ? 600 : cyScreen;

    HINSTANCE hInstance = CApp::GetApp()->GetInstance();

    WNDCLASS cls;
    if (!GetClassInfo(hInstance, m_wstrClsName.c_str(), &cls)) Register();

    wstring wstrTitle(L"3D");
    m_hwnd = CreateWindow(m_wstrClsName.c_str(), wstrTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        (cxScreen - width) / 2, (cyScreen - height) / 2, width, height,
        NULL, NULL, hInstance, this);
    if (!m_hwnd) {
        MessageBox(NULL, L"Create window failed", L"Error", MB_OK);
        return FALSE;
    }
    else {
        BOOL enable;
        DwmIsCompositionEnabled(&enable);
        if (enable) {
            DWMNCRENDERINGPOLICY policy = DWMNCRP_ENABLED;
            DwmSetWindowAttribute(m_hwnd, DWMWA_NCRENDERING_POLICY, &policy, sizeof(DWMNCRENDERINGPOLICY));
        }
    }

    RECT rc;
    GetClientRect(m_hwnd, &rc);

    return TRUE;
}

LRESULT CMainWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        HANDLE_MSG(hWnd, WM_ERASEBKGND, OnEraseBkgnd);
        break;
    default:
        return CXWnd::WndProc(hWnd, uMsg, wParam, lParam);
    }

    return 0L;
}

BOOL CMainWnd::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    BOOL bRet = CXWnd::OnCreate(hwnd, lpCreateStruct);
    if (!bRet) return bRet;

    ComPtr<IDXGIFactory1> spFactory;
    ComPtr<IDXGIFactory3> spFactory3;
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&spFactory));
    if (FAILED(hr)) {
        MessageBox(hwnd, L"Create DXGI Factory Failed!", L"D3D11", MB_OK);
        return FALSE;
    }
    hr = spFactory.Get()->QueryInterface(IID_PPV_ARGS(&spFactory3));

    UINT uFlag = 0;
#ifdef _DEBUG
    uFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, uFlag, NULL, 0, D3D11_SDK_VERSION, &m_spD3D11, NULL, &m_spImCtx);
    if (FAILED(hr)) {
        MessageBox(hwnd, L"Create Device Failed!", L"D3D11", MB_OK);
        return FALSE;
    }

    DXGI_SWAP_CHAIN_DESC1 desc;
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC1));
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Stereo = FALSE;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = 1;
    desc.Scaling = DXGI_SCALING_STRETCH;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC descFS;
    ZeroMemory(&descFS, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));
    descFS.RefreshRate.Numerator = 60;
    descFS.RefreshRate.Denominator = 1;
    descFS.Windowed = TRUE;

    hr = spFactory3->CreateSwapChainForHwnd(m_spD3D11.Get(), hwnd, &desc, &descFS, NULL, &m_spSwapChain);
    if (FAILED(hr)) {
        MessageBox(hwnd, L"Create Swap Chain Failed!", L"D3D11", MB_OK);
        return FALSE;
    }

    ComPtr<ID3D11Texture2D> spBackBuffer;
    hr = m_spSwapChain->GetBuffer(0, IID_PPV_ARGS(&spBackBuffer));
    hr = m_spD3D11->CreateRenderTargetView(spBackBuffer.Get(), NULL, &m_spRTV);

    ID3D11RenderTargetView *pRTV = m_spRTV.Get();
    m_spImCtx->OMSetRenderTargets(1, &pRTV, NULL);
    if (FAILED(hr)) {
        MessageBox(hwnd, L"Create Render Target View Failed!", L"D3D11", MB_OK);
        return FALSE;
    }

    D3D11_VIEWPORT viewPort;
    viewPort.TopLeftX = 0.0f;
    viewPort.TopLeftY = 0.0f;
    viewPort.Width = static_cast<float>(lpCreateStruct->cx);
    viewPort.Height = static_cast<float>(lpCreateStruct->cy);
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;
    m_spImCtx->RSSetViewports(1, &viewPort);

    return TRUE;
}

void CMainWnd::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    DXGI_MODE_DESC desc;
    ZeroMemory(&desc, sizeof(DXGI_MODE_DESC));
    desc.Width = cx;
    desc.Height = cy;
    desc.RefreshRate.Numerator = 60;
    desc.RefreshRate.Denominator = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Scaling = DXGI_MODE_SCALING_STRETCHED;

    HRESULT hr = m_spSwapChain->ResizeTarget(&desc);
}

BOOL CMainWnd::OnEraseBkgnd(HWND hwnd, HDC hdc)
{
    m_spImCtx->ClearRenderTargetView(m_spRTV.Get(), Colors::DarkBlue);
    return TRUE;
}

void CMainWnd::OnPaint(HWND hwnd)
{
    CXWnd::OnPaint(hwnd);
    HRESULT hr = m_spSwapChain->Present(1, 0);
}