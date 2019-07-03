#include "stdafx.h"
#include "XWnd.h"
#include "App.h"

CXWnd::CXWnd()
    :m_wstrClsName(L"CXWnd")
{
}


CXWnd::~CXWnd()
{
}

ATOM CXWnd::Register()
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

    return RegisterClass(&wcls);
}

BOOL CXWnd::Create(wstring &wstrTitle, const RECT &rect, const CXWnd &xwndParent,DWORD dwStyle)
{
    HINSTANCE hInstance = CApp::GetApp()->GetInstance();

    WNDCLASS cls;
    if (!GetClassInfo(hInstance, m_wstrClsName.c_str(), &cls)) Register();

    m_hwnd = CreateWindow(m_wstrClsName.c_str(), wstrTitle.c_str(),
        dwStyle|WS_CHILD|WS_VISIBLE|WS_BORDER,
        rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
        xwndParent, NULL, hInstance, this);
    if (!m_hwnd) {
        MessageBox(xwndParent, L"Create window failed", L"Error", MB_OK);
        return FALSE;
    }
    else{
        return TRUE;
    }
}

void CXWnd::Destroy()
{
    DestroyWindow(m_hwnd);
    m_hwnd = NULL;
}

LRESULT CXWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hWnd, WM_SIZE, OnSize);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0L;
}

LRESULT CXWnd::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CXWnd *pXWnd = NULL;
    if (uMsg == WM_CREATE) {
        LPVOID lpCreateParams = reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams;
        pXWnd = reinterpret_cast<CXWnd*>(lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pXWnd));
    }
    else {
        pXWnd = reinterpret_cast<CXWnd*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if(!pXWnd) return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return pXWnd->WndProc(hWnd, uMsg, wParam, lParam);
}

BOOL CXWnd::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(LOGFONT));
    lf.lfHeight = 12;
    lf.lfWeight = FW_MEDIUM;
    wcscpy_s(lf.lfFaceName, 32, L"ËÎÌו");
    m_hfontSong = CreateFontIndirect(&lf);

    return TRUE;
}

void CXWnd::OnDestroy(HWND hwnd)
{
    DeleteFont(m_hfontSong);
    PostQuitMessage(0);
}

void CXWnd::OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    EndPaint(hwnd, &ps);
}

void CXWnd::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
}

void CXWnd::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, DefWindowProc);
}