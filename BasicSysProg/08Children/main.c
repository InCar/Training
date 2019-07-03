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

    HWND hWndMain = CreateWindow(wszTitle, L"子窗口", WS_OVERLAPPEDWINDOW, (cxScreen - width) / 2, (cyScreen - height) / 2, width, height, NULL, NULL, hInstance, NULL);
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
    HWND hStaticUser;
    HWND hEditUser;
    HWND hStaticPwd;
    HWND hEditPwd;
    HWND hBtnLogin;
    HFONT hFontText;
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

    g_app.swLeft.pAPI->Create(&g_app.swLeft, hwnd, &rcLeft, L"左");
    g_app.swRight.pAPI->Create(&g_app.swRight, hwnd, &rcRight, L"右");

    // Controls
    INITCOMMONCONTROLSEX ccex;
    ccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    ccex.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&ccex);

    LOGFONT lf;
    ZeroMemory(&lf, sizeof(LOGFONT));
    lf.lfHeight = 16;
    wcscpy_s(lf.lfFaceName, 32, L"宋体");
    g_app.hFontText = CreateFontIndirect(&lf);

    int x = rcLeft.left, y = rcLeft.bottom;
    g_app.hStaticUser = CreateWindow(L"STATIC", L"用户名", WS_CHILD | WS_VISIBLE | SS_RIGHT, x + 8, y + 8, 74, 18, hwnd, NULL, NULL, NULL);
    g_app.hEditUser = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_LEFT, x + 8 + 74 + 8, y + 8, 240, 18, hwnd, NULL, NULL, NULL);
    g_app.hStaticPwd = CreateWindow(L"STATIC", L"密码", WS_CHILD | WS_VISIBLE | SS_RIGHT, x + 8, y + 8 * 2 + 18, 74, 18, hwnd, NULL, NULL, NULL);
    g_app.hEditPwd = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_LEFT | ES_PASSWORD, x + 8 + 74 + 8, y + 8 * 2 + 18, 240, 18, hwnd, NULL, NULL, NULL);
    g_app.hBtnLogin = CreateWindow(L"BUTTON", L"登录", WS_CHILD | WS_VISIBLE | BS_CENTER, x + 8 + 74 + 8, y + 8 * 3 + 18 * 2, 74, 28, hwnd, NULL, NULL, NULL);

    ShowWindow(hwnd, SW_SHOW);
    return TRUE;
}

void OnDestroy(HWND hwnd)
{
    DeleteFont(g_app.hFontText);
    PostQuitMessage(0);
}

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    EndPaint(hwnd, &ps);
}

HBRUSH OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{
    SelectFont(hdc, g_app.hFontText);
    if (hwndChild == g_app.hStaticUser || hwndChild == g_app.hStaticPwd) {
        SetBkMode(hdc, TRANSPARENT);
        return GetStockBrush(LTGRAY_BRUSH);
    }

    return GetStockBrush(WHITE_BRUSH);
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    if (hwndCtl == g_app.hBtnLogin) {
        if (codeNotify == BN_CLICKED) {
            wchar_t wszUser[32], wszPwd[32], buf[256];
            Edit_GetText(g_app.hEditUser, wszUser, 32);
            Edit_GetText(g_app.hEditPwd, wszPwd, 32);
            StringCchPrintf(buf, 256, L"Login with %s - %s", wszUser, wszPwd);
            MessageBox(hwnd, buf, L"登录信息", MB_OK);
        }
    }
    FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, DefWindowProc);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hWnd, WM_CTLCOLORSTATIC, OnCtlColor);
        HANDLE_MSG(hWnd, WM_CTLCOLOREDIT, OnCtlColor);
        HANDLE_MSG(hWnd, WM_CTLCOLORBTN, OnCtlColor);
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0L;
}