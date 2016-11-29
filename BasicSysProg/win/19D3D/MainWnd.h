#pragma once
#include "XWnd.h"
#include "resource.h"

using namespace Microsoft::WRL;

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
    virtual void OnSize(HWND hwnd, UINT state, int cx, int cy);
    virtual BOOL OnEraseBkgnd(HWND hwnd, HDC hdc);
    virtual void OnPaint(HWND hwnd);
private:
    ComPtr<ID3D11Device> m_spD3D11;
    ComPtr<ID3D11DeviceContext> m_spImCtx;
    ComPtr<IDXGISwapChain1> m_spSwapChain;
    ComPtr<ID3D11RenderTargetView> m_spRTV;
};

