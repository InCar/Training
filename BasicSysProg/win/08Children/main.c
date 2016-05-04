#include "stdafx.h"
#include "SimpleWnd.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
    wchar_t wszTitle[] = L"Children";

    WNDCLASS wcls;
    ZeroMemory(&wcls, sizeof(WNDCLASS));
    wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcls.lpfnWndProc = WindowProc;
    wcls.hInstance = hInstance;
    wcls.hbrBackground = GetStockObject(LTGRAY_BRUSH);
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

    HWND hWndMain = CreateWindow(wszTitle, L"◊”¥∞ø⁄", WS_OVERLAPPEDWINDOW, (cxScreen - width) / 2, (cyScreen - height) / 2, width, height, NULL, NULL, hInstance, NULL);
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
    SimpleWnd swLeft;
    SimpleWnd swRight;
} g_app;

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    SimpleWndInit(&g_app.swLeft);
    SimpleWndInit(&g_app.swRight);

    RECT rc, rcLeft, rcRight;
    GetClientRect(hwnd, &rc);
    rcLeft = rc, rcRight = rc;
    rcLeft.right = (rc.right - rc.left) / 2, rcLeft.bottom = (rc.bottom - rc.top)/2;
    rcRight.left = rcLeft.right, rcRight.bottom = rcLeft.bottom;

    g_app.swLeft.pAPI->Create(&g_app.swLeft, hwnd, &rcLeft, L"◊Û");
    g_app.swRight.pAPI->Create(&g_app.swRight, hwnd, &rcRight, L"”“");

    ShowWindow(hwnd, SW_SHOW);
    return TRUE;
}

void OnDestroy(HWND hwnd)
{
    PostQuitMessage(0);
}

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0L;
}