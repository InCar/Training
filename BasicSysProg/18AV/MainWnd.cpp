#include "stdafx.h"
#include "MainWnd.h"
#include "App.h"

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

    wstring wstrTitle(L"ÒôÆµ&ÊÓÆµ");
    m_hwnd = CreateWindow(m_wstrClsName.c_str(), wstrTitle.c_str(), WS_OVERLAPPEDWINDOW|WS_VISIBLE,
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

    // SineWnd
    wstring wstrSine = L"ÕýÏÒ²¨";
    RECT rcSine;
    rcSine.left = rcSine.top = 0;
    rcSine.right = rc.right / 2;
    rcSine.bottom = rc.bottom / 2;
    if (rcSine.bottom > 128) rcSine.bottom = 128;
    m_wndSine.Create(wstrSine, rcSine, *this, 0);

    // MP3Player
    wstring wstrMP3 = L"MP3Player";
    RECT rcMP3;
    rcMP3.left = rcSine.right - 1;
    rcMP3.top = rcSine.top;
    rcMP3.right = rc.right;
    rcMP3.bottom = rcSine.bottom;
    m_wndMP3Player.Create(wstrMP3, rcMP3, *this, 0);

    // MovPlayer
    wstring wstrMov = L"MovPlayer";
    RECT rcMov;
    rcMov.left = rc.left;
    rcMov.top = rcSine.bottom - 1;
    rcMov.right = rc.right;
    rcMov.bottom = rc.bottom;
    m_wndMovPlayer.Create(wstrMov, rcMov, *this, 0);

    return TRUE;
}

void CMainWnd::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    RECT rc;
    GetClientRect(m_hwnd, &rc);

    RECT rcSine;
    rcSine.left = rcSine.top = 0;
    rcSine.right = rc.right / 2;
    rcSine.bottom = rc.bottom / 2;
    if (rcSine.bottom > 128) rcSine.bottom = 128;
    SetWindowPos(m_wndSine, 0, rcSine.left, rcSine.top, rcSine.right - rcSine.left, rcSine.bottom - rcSine.top, SWP_NOZORDER);

    RECT rcMP3;
    rcMP3.left = rcSine.right - 1;
    rcMP3.top = rcSine.top;
    rcMP3.right = rc.right;
    rcMP3.bottom = rcSine.bottom;
    SetWindowPos(m_wndMP3Player, 0, rcMP3.left, rcMP3.top, rcMP3.right - rcMP3.left, rcMP3.bottom - rcMP3.top, SWP_NOZORDER);

    RECT rcMov;
    rcMov.left = rc.left;
    rcMov.top = rcSine.bottom - 1;
    rcMov.right = rc.right;
    rcMov.bottom = rc.bottom;
    SetWindowPos(m_wndMovPlayer, 0, rcMov.left, rcMov.top, rcMov.right - rcMov.left, rcMov.bottom - rcMov.top, SWP_NOZORDER);
}