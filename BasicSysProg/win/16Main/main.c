#include "stdafx.h"
#include "resource.h"

#include "../16SA/16SA.h"
#include "../16SD/16SD.h"

typedef int(*fnCall)(wchar_t *pwszBuf, int count);

HFONT g_fontMain = NULL;
wchar_t g_wszBuf[64];

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void ShowLastError(HWND, DWORD);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
    wchar_t wszTitle[] = L"Library";

    WNDCLASS wcls;
    ZeroMemory(&wcls, sizeof(WNDCLASS));
    wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcls.lpfnWndProc = WindowProc;
    wcls.hInstance = hInstance;
    wcls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcls.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_APP));
    wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcls.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_MAIN);
    wcls.lpszClassName = wszTitle;

    if (!RegisterClass(&wcls)) {
        MessageBox(NULL, L"Register class failed", L"Error", MB_OK);
        return GetLastError();
    }

    int cxScreen = GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);
    int width = cxScreen > 800 ? 800 : cxScreen;
    int height = cyScreen > 600 ? 600 : cyScreen;

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        MessageBox(NULL, L"CoInitialize failed", L"Error", MB_OK);
        return GetLastError();
    }

    HWND hwndMain = CreateWindow(wszTitle, L"¶¯Ì¬Á´½Ó¿â", WS_OVERLAPPEDWINDOW, (cxScreen - width) / 2, (cyScreen - height) / 2, width, height, NULL, NULL, hInstance, NULL);
    if (!hwndMain) {
        MessageBox(NULL, L"Create window failed", L"Error", MB_OK);
        return GetLastError();
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CoUninitialize();

    return (int)msg.wParam;
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(LOGFONT));
    lf.lfHeight = 32;
    lf.lfWeight = FW_HEAVY;
    wcscpy_s(lf.lfFaceName, 32, L"Engravers MT");
    g_fontMain = CreateFontIndirect(&lf);

    UpdateWindow(hwnd);
    ShowWindow(hwnd, SW_SHOW);

    return TRUE;
}

void OnDestroy(HWND hwnd)
{
    DeleteFont(g_fontMain);

    PostQuitMessage(0);
}

void OnPaint(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    SetTextColor(hdc, RGB(0xa0, 0x90, 0x40));
    SetBkMode(hdc, TRANSPARENT);
    SelectFont(hdc, g_fontMain);

    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    rc.top = rc.bottom / 2 - tm.tmHeight / 2;

    DrawText(hdc, g_wszBuf, (int)wcslen(g_wszBuf), &rc, DT_CENTER);

    EndPaint(hwnd, &ps);
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id) {
    case ID_RUN_STATIC:
    {
        CallStaticLib(g_wszBuf, sizeof(g_wszBuf)/sizeof(wchar_t));
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
    case ID_RUN_DYNAMIC:
    {
        CallDynamicLib(g_wszBuf, sizeof(g_wszBuf) / sizeof(wchar_t));
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
    case ID_RUN_LOAD:
    {
        if (LoadLibrary(L"16SL") == NULL) {
            ShowLastError(hwnd, GetLastError());
        }
        break;
    }
    case ID_RUN_FREE:
    {
        if (FreeLibrary(GetModuleHandle(L"16SL")) == FALSE) {
            ShowLastError(hwnd, GetLastError());
        }
        break;
    }
    case ID_RUN_EXEC:
    {
        HMODULE h16SL = GetModuleHandle(L"16SL");
        if (h16SL != NULL) {
            fnCall pfnCall = (fnCall)GetProcAddress(h16SL, "CallDynamicLib");
            pfnCall(g_wszBuf, sizeof(g_wszBuf) / sizeof(wchar_t));
            InvalidateRect(hwnd, NULL, TRUE);
        }
        else {
            ShowLastError(hwnd, GetLastError());
        }
        break;
    }
    case ID_FILE_EXIT:
    {
        PostQuitMessage(0);
        break;
    }
    default:
        FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, DefWindowProc);
    }
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0L;
}

void ShowLastError(HWND hwnd, DWORD dwError)
{
    wchar_t *pwszBuffer = NULL;

    if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, dwError, MAKELANGID(LANG_CHINESE_SIMPLIFIED, SUBLANG_CHINESE_SIMPLIFIED), (LPWSTR)&pwszBuffer, 0, NULL) > 0) {
        MessageBox(hwnd, pwszBuffer, L"Error", MB_OK);

        LocalFree(pwszBuffer);
    }

}