#include "stdafx.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
    wchar_t wszTitle[] = L"Timer";

    WNDCLASS wcls;
    ZeroMemory(&wcls, sizeof(WNDCLASS));
    wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcls.lpfnWndProc = WindowProc;
    wcls.hInstance = hInstance;
    wcls.hbrBackground = GetStockObject(BLACK_BRUSH);
    wcls.hIcon = LoadIcon(NULL, IDI_SHIELD);
    wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcls.lpszClassName = wszTitle;

    if (!RegisterClass(&wcls)) {
        MessageBox(NULL, L"Register class failed", L"Error", MB_OK);
        return GetLastError();
    }

    int cxScreen = GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);
    int width = cxScreen > 800 ? 800 : cxScreen;
    int height = cyScreen > 600 ? 600 : cyScreen;

    HWND hWndMain = CreateWindow(wszTitle, L"¶¨Ê±Æ÷", WS_OVERLAPPEDWINDOW, (cxScreen - width) / 2, (cyScreen - height) / 2, width, height, NULL, NULL, hInstance, NULL);
    if (!hWndMain) {
        MessageBox(NULL, L"Create window failed", L"Error", MB_OK);
        return GetLastError();
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

struct APP
{
    int nTx;
    HFONT hFont;
    HPEN hPen;
    RECT rcText;

    HDC hdcMem;
    HBITMAP hBmp;
} g_app;

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(LOGFONT));
    
    lf.lfHeight = 128;
    lf.lfWeight = FW_HEAVY;
    wcscpy_s(lf.lfFaceName, 32, L"agency fb");
    
    // lf.lfHeight = 96;
    // lf.lfWeight = FW_HEAVY;
    // wcscpy_s(lf.lfFaceName, 32, L"consolas");
    
    g_app.hFont = CreateFontIndirect(&lf);
    g_app.hPen = CreatePen(PS_SOLID, 1, RGB(0x80, 0x00, 0x00));

    HDC hdc = GetDC(hwnd);
    g_app.hdcMem = CreateCompatibleDC(hdc);
    g_app.hBmp = CreateCompatibleBitmap(hdc, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    SelectBitmap(g_app.hdcMem, g_app.hBmp);
    ReleaseDC(hwnd, hdc);

    SetTimer(hwnd, 1, 1, NULL);
    SetTimer(hwnd, 2, 10, NULL);
    SetTimer(hwnd, 3, 100, NULL);
    SetTimer(hwnd, 4, 1000, NULL);

    ShowWindow(hwnd, SW_SHOW);
    return TRUE;
}

void OnDestroy(HWND hwnd)
{
    for (int i = 1; i <= 4; i++)
        KillTimer(hwnd, i);
    DeleteFont(g_app.hFont);
    DeletePen(g_app.hPen);
    DeleteDC(g_app.hdcMem);
    DeleteBitmap(g_app.hBmp);
    PostQuitMessage(0);
}

void OnTimer(HWND hwnd, UINT id)
{
    if(id == g_app.nTx)
        InvalidateRect(hwnd, &g_app.rcText, FALSE);
}

void OnPaint(HWND hwnd)
{
    static TEXTMETRIC tm;

    RECT rc;
    GetClientRect(hwnd, &rc);

    SYSTEMTIME tmNow;
    GetLocalTime(&tmNow);
    wchar_t wszBuf[128];
    wsprintf(wszBuf, L"%02d:%02d:%02d.%03d", tmNow.wHour, tmNow.wMinute, tmNow.wSecond, tmNow.wMilliseconds);

    SelectFont(g_app.hdcMem, g_app.hFont);
    SelectPen(g_app.hdcMem, g_app.hPen);
    SetBkMode(g_app.hdcMem, TRANSPARENT);
    SetTextColor(g_app.hdcMem, RGB(0x66, 0x00, 0x00));

    SIZE size;
    GetTextExtentPoint32(g_app.hdcMem, wszBuf, (int)wcslen(wszBuf), &size);

    if (!tm.tmHeight) {
        GetTextMetrics(g_app.hdcMem, &tm);
        g_app.nTx = 3;
    }

    g_app.rcText = rc;
    g_app.rcText.top = rc.bottom / 2 - tm.tmHeight / 2;
    g_app.rcText.left = rc.right / 2 - tm.tmAveCharWidth * 6;
    g_app.rcText.bottom = g_app.rcText.top + size.cy;
    g_app.rcText.right = g_app.rcText.left + size.cx;
    
    FillRect(g_app.hdcMem, &rc, GetStockBrush(BLACK_BRUSH));
    DrawText(g_app.hdcMem, wszBuf, (int)wcslen(wszBuf), &g_app.rcText, DT_LEFT);

    MoveToEx(g_app.hdcMem, g_app.rcText.left, g_app.rcText.bottom, NULL);
    LineTo(g_app.hdcMem, g_app.rcText.right, g_app.rcText.bottom);
    
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    BitBlt(hdc, 0, 0, rc.right, rc.bottom, g_app.hdcMem, 0, 0, SRCCOPY);
    EndPaint(hwnd, &ps);
}

void OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
    g_app.nTx = (g_app.nTx++) % 5;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hWnd, WM_TIMER, OnTimer);
        HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hWnd, WM_KEYDOWN, OnKeyDown);
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0L;
}