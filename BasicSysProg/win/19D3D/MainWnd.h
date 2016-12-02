#pragma once
#include "XWnd.h"
#include "resource.h"
#include "Cube.h"
#include "Cube2.h"

using namespace Microsoft::WRL;

struct ConstantBuffer
{
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;

    XMFLOAT4 vLightDir;
    XMFLOAT4 vLightColor;
};

class CMainWnd : public CXWnd
{
public:
    CMainWnd();
    virtual ~CMainWnd();

    virtual ATOM Register();
    BOOL Create();
protected:
    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
    virtual void OnDestroy(HWND hwnd);
    virtual void OnSize(HWND hwnd, UINT state, int cx, int cy);
    virtual BOOL OnEraseBkgnd(HWND hwnd, HDC hdc);
    virtual void OnPaint(HWND hwnd);
    virtual void OnTimer(HWND hwnd, UINT id);
private:
    ComPtr<ID3D11Device> m_spD3D11;
    ComPtr<ID3D11DeviceContext> m_spImCtx;
    ComPtr<IDXGISwapChain1> m_spSwapChain;
    ComPtr<ID3D11Texture2D> m_spZ;
    ComPtr<ID3D11DepthStencilView> m_spZView;
    ComPtr<ID3D11RenderTargetView> m_spRTV;
    ComPtr<ID3D11InputLayout> m_spIL;
    ComPtr<ID3D11Buffer> m_spConstant;

    CCube2 *m_pCube;

    BYTE *m_pVS, *m_pPS;
    DWORD m_dwVS, m_dwPS;
    ComPtr<ID3D11VertexShader> m_spVS;
    ComPtr<ID3D11PixelShader> m_spPS;
    HRESULT LoadShader();

    ConstantBuffer m_cb;
    ULONGLONG m_u64Begin;
};
