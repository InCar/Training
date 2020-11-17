#include "stdafx.h"
#include "MainWnd.h"
#include "App.h"

using namespace DirectX;

CMainWnd::CMainWnd()
{
    m_wstrClsName.assign(L"MainWnd");
    m_pCube = NULL;
    m_pVS = m_pPS = NULL;
    m_dwVS = m_dwPS = 0;
}


CMainWnd::~CMainWnd()
{
    if (m_pVS) delete[] m_pVS;
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

    UpdateWindow(m_hwnd);

    return TRUE;
}

LRESULT CMainWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        HANDLE_MSG(hWnd, WM_TIMER, OnTimer);
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

    // 创建D3D设备
    UINT uFlag = 0;
#ifdef _DEBUG
    uFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, uFlag, NULL, 0, D3D11_SDK_VERSION, &m_spD3D11, NULL, &m_spImCtx);
    if (FAILED(hr)) {
        MessageBox(hwnd, L"Create Device Failed!", L"D3D11", MB_OK);
        return FALSE;
    }

    // 创建交换链
    DXGI_SWAP_CHAIN_DESC1 desc;
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC1));
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Stereo = FALSE;
    desc.SampleDesc.Count = 4;
    desc.SampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_PATTERN;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = 1;
    desc.Scaling = DXGI_SCALING_STRETCH;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    hr = spFactory3->CreateSwapChainForHwnd(m_spD3D11.Get(), hwnd, &desc, NULL, NULL, &m_spSwapChain);
    if (FAILED(hr)) {
        MessageBox(hwnd, L"Create Swap Chain Failed!", L"D3D11", MB_OK);
        return FALSE;
    }

    // 光栅参数
    D3D11_RASTERIZER_DESC descRS;
    ZeroMemory(&descRS, sizeof(D3D11_RASTERIZER_DESC));
    descRS.FillMode = D3D11_FILL_SOLID;
    descRS.CullMode = D3D11_CULL_BACK;
    descRS.DepthClipEnable = TRUE;
    descRS.MultisampleEnable = TRUE;

    ComPtr<ID3D11RasterizerState> spRS;
    hr = m_spD3D11->CreateRasterizerState(&descRS, &spRS);
    m_spImCtx->RSSetState(spRS.Get());

    // GPU
    hr = LoadShader();
    if (FAILED(hr)) {
        MessageBox(hwnd, L"Create Shader Failed!", L"D3D11", MB_OK);
        return FALSE;
    }

    // 数据流格式
    // D3D11_INPUT_ELEMENT_DESC layout[] = {
    //     { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //     { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA , 0 }
    // };
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA , 0 }
    };
    hr = m_spD3D11->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), m_pVS, m_dwVS, &m_spIL);
    m_spImCtx->IASetInputLayout(m_spIL.Get());

    // 立方体
    m_pCube = new CCube2(m_spD3D11);
    hr = m_pCube->Init();

    UINT stride = m_pCube->GetStride();
    UINT offset = 0;
    ID3D11Buffer *pVertexBuffer = m_pCube->GetVertexes().Get();
    m_spImCtx->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
    m_spImCtx->IASetIndexBuffer(m_pCube->GetIndex().Get(), DXGI_FORMAT_R32_UINT, 0);
    m_spImCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 3D空间
    D3D11_BUFFER_DESC descWorld;
    ZeroMemory(&descWorld, sizeof(D3D11_BUFFER_DESC));
    descWorld.Usage = D3D11_USAGE_DEFAULT;
    descWorld.ByteWidth = sizeof(ConstantBuffer);
    descWorld.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = m_spD3D11->CreateBuffer(&descWorld, NULL, &m_spConstant);

    // 标准空间
    m_cb.mWorld = ::XMMatrixIdentity();

    // 摄像机位
    XMVECTOR Eye = XMVectorSet(3.0f, 8.0f, -15.0f, 0.0f);
    XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    m_cb.mView = ::XMMatrixLookAtLH(Eye, At, Up);

    // 灯光
    m_cb.vLightDir = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
    m_cb.vLightColor = XMFLOAT4(0.5f, 0.5f, 0.3f, 1.0f);

	OnSize(hwnd, 0, lpCreateStruct->cx, lpCreateStruct->cy);

    // 开始时间标定
    m_u64Begin = GetTickCount64();
    SetTimer(hwnd, 1, 33, NULL);

    return TRUE;
}

void CMainWnd::OnDestroy(HWND hwnd)
{
    delete m_pCube;
    m_pCube = NULL;

    CXWnd::OnDestroy(hwnd);
}

void CMainWnd::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    // 尺寸不能是0
    if (cx == 0) cx = 1;
    if (cy == 0) cy = 1;

    // 释放先前的
    m_spRTV = nullptr;
    m_spZView = nullptr;
    m_spZ = nullptr;

    // resize
    HRESULT hr = m_spSwapChain->ResizeBuffers(0, cx, cy, DXGI_FORMAT_UNKNOWN, 0);

	// 设定渲染目标
	ComPtr<ID3D11Texture2D> spBackBuffer;
	hr = m_spSwapChain->GetBuffer(0, IID_PPV_ARGS(&spBackBuffer));
	hr = m_spD3D11->CreateRenderTargetView(spBackBuffer.Get(), NULL, &m_spRTV);

	// Z-Buffer
	D3D11_TEXTURE2D_DESC descZ;
	ZeroMemory(&descZ, sizeof(D3D11_TEXTURE2D_DESC));
	descZ.Width = cx;
	descZ.Height = cy;
	descZ.MipLevels = 1;
	descZ.ArraySize = 1;
	descZ.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descZ.SampleDesc.Count = 4;
	descZ.SampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_PATTERN;
	descZ.Usage = D3D11_USAGE_DEFAULT;
	descZ.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = m_spD3D11->CreateTexture2D(&descZ, NULL, &m_spZ);

	D3D11_DEPTH_STENCIL_VIEW_DESC descZView;
	ZeroMemory(&descZView, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	descZView.Format = descZ.Format;
	descZView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hr = m_spD3D11->CreateDepthStencilView(m_spZ.Get(), &descZView, &m_spZView);

	ID3D11RenderTargetView* pRTV = m_spRTV.Get();
	m_spImCtx->OMSetRenderTargets(1, &pRTV, m_spZView.Get());

	// 设定可见区域
	D3D11_VIEWPORT viewPort;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	viewPort.Width = static_cast<float>(cx);
	viewPort.Height = static_cast<float>(cy);
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	m_spImCtx->RSSetViewports(1, &viewPort);

	// 镜头
	m_cb.mProjection = ::XMMatrixPerspectiveFovLH(XM_PI / 6.0f, (cx) / (FLOAT)(cy), 0.01f, 100.0f);
}

BOOL CMainWnd::OnEraseBkgnd(HWND hwnd, HDC hdc)
{
    return TRUE;
}

void CMainWnd::OnPaint(HWND hwnd)
{
    CXWnd::OnPaint(hwnd);

    m_spImCtx->ClearRenderTargetView(m_spRTV.Get(), Colors::Blue);
    m_spImCtx->ClearDepthStencilView(m_spZView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    // set render target
    m_spImCtx->OMSetRenderTargets(1, m_spRTV.GetAddressOf(), m_spZView.Get());

    ConstantBuffer cb1;
    cb1.mWorld = XMMatrixTranspose(m_cb.mWorld);
    cb1.mView = XMMatrixTranspose(m_cb.mView);
    cb1.mProjection = XMMatrixTranspose(m_cb.mProjection);
    cb1.vLightDir = m_cb.vLightDir;
    cb1.vLightColor = m_cb.vLightColor;
    m_spImCtx->UpdateSubresource(m_spConstant.Get(), 0, nullptr, &cb1, 0, 0);
    ID3D11Buffer *pConstant = m_spConstant.Get();
    m_spImCtx->VSSetConstantBuffers(0, 1, &pConstant);
    m_spImCtx->VSSetShader(m_spVS.Get(), NULL, 0);
    m_spImCtx->PSSetConstantBuffers(0, 1, &pConstant);
    m_spImCtx->PSSetShader(m_spPS.Get(), NULL, 0);
    m_spImCtx->DrawIndexed(m_pCube->GetVertexesCount(), 0, 0);

    ConstantBuffer cb2;
    cb2.mWorld = XMMatrixTranspose(m_cb.mWorld
        * XMMatrixScaling(0.3f, 0.3f, 0.3f)
        * XMMatrixRotationX((GetTickCount64() - m_u64Begin) / 1000.0f)
        * XMMatrixTranslation(+4.0f, 0.0f, 0.0f)
        * XMMatrixRotationY((GetTickCount64() - m_u64Begin) / -600.0f));
    cb2.mView = XMMatrixTranspose(m_cb.mView);
    cb2.mProjection = XMMatrixTranspose(m_cb.mProjection);
    cb2.vLightDir = m_cb.vLightDir;
	cb2.vLightColor = m_cb.vLightColor;
    m_spImCtx->UpdateSubresource(m_spConstant.Get(), 0, nullptr, &cb2, 0, 0);
    m_spImCtx->DrawIndexed(m_pCube->GetVertexesCount(), 0, 0);

    HRESULT hr = m_spSwapChain->Present(1, 0);
}

void CMainWnd::OnTimer(HWND hwnd, UINT id)
{
    m_cb.mWorld = XMMatrixRotationY((GetTickCount64() - m_u64Begin) / 1000.0f);
    InvalidateRect(hwnd, NULL, TRUE);
}

HRESULT CMainWnd::LoadShader()
{
    wchar_t buf[512];
    GetModuleFileName(NULL, buf, 512);
    wchar_t *pLast = wcsrchr(buf, L'\\');
    if (pLast == NULL) return E_FAIL;
    wcscpy_s(pLast + 1, 512 - ((pLast + 1) - buf), L"VS2.cso");

    HANDLE hCSO = CreateFile(buf, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hCSO == INVALID_HANDLE_VALUE) return E_HANDLE;
    DWORD dwSize = GetFileSize(hCSO, NULL);
    m_pVS = new BYTE[dwSize];
    ReadFile(hCSO, m_pVS, dwSize, &m_dwVS, NULL);
    CloseHandle(hCSO);

    HRESULT hr = m_spD3D11->CreateVertexShader(m_pVS, m_dwVS, NULL, &m_spVS);
    if (FAILED(hr)) return hr;

    wcscpy_s(pLast + 1, 512 - ((pLast + 1) - buf), L"PS2.cso");
    hCSO = CreateFile(buf, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hCSO == INVALID_HANDLE_VALUE) return E_HANDLE;
    dwSize = GetFileSize(hCSO, NULL);
    m_pPS = new BYTE[dwSize];
    ReadFile(hCSO, m_pPS, dwSize, &m_dwPS, NULL);
    CloseHandle(hCSO);

    hr = m_spD3D11->CreatePixelShader(m_pPS, m_dwPS, NULL, &m_spPS);
    if (FAILED(hr)) return hr;

    return S_OK;
}