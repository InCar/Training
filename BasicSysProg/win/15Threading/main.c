#include "stdafx.h"
#include "resource.h"

DWORD g_dwTotal_shared = 0;
BOOL g_bShouldExit_shared = FALSE;
HWND g_hwndMain = NULL;
HDC g_hdcMem = NULL;
HFONT g_fontNumber = NULL;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void ShowLastError(HWND, DWORD);
DWORD WINAPI CountNumber(LPVOID pArgs);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
    wchar_t wszTitle[] = L"Threading";

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

    g_hwndMain = CreateWindow(wszTitle, L"多线程", WS_OVERLAPPEDWINDOW, (cxScreen - width) / 2, (cyScreen - height) / 2, width, height, NULL, NULL, hInstance, NULL);
    if (!g_hwndMain) {
        MessageBox(NULL, L"Create window failed", L"Error", MB_OK);
        return GetLastError();
    }

    HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(g_hwndMain, hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    DestroyAcceleratorTable(hAccel);

    CoUninitialize();

    return (int)msg.wParam;
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    // 最大可能的桌面尺寸
    int maxWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int maxHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    LOGFONT lf;
    ZeroMemory(&lf, sizeof(LOGFONT));
    lf.lfHeight = 128;
    lf.lfWeight = FW_HEAVY;
    wcscpy_s(lf.lfFaceName, 32, L"Engravers MT");
    g_fontNumber = CreateFontIndirect(&lf);

    // 双缓冲
    HDC hdc = GetDC(hwnd);
    g_hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hBMP = CreateCompatibleBitmap(hdc, maxWidth, maxHeight);
    SelectBitmap(g_hdcMem, hBMP);
    ReleaseDC(hwnd, hdc);

    SelectFont(g_hdcMem, g_fontNumber);
    SetBkMode(g_hdcMem, TRANSPARENT);
    SetTextColor(g_hdcMem, RGB(0x40, 0xd4, 0x40));

    ShowWindow(hwnd, SW_SHOW);

    return TRUE;
}

void OnDestroy(HWND hwnd)
{
    DeleteFont(g_fontNumber);
    DeleteDC(g_hdcMem);

    PostQuitMessage(0);
}

void OnPaint(HWND hwnd)
{
    static wchar_t wszBuf[64];
    StringCchPrintf(wszBuf, 64-1, L"%d\r\n", g_dwTotal_shared);

    RECT rc;
    GetClientRect(hwnd, &rc);
    FillRect(g_hdcMem, &rc, GetStockBrush(BLACK_BRUSH));
    DrawText(g_hdcMem, wszBuf, (int)wcslen(wszBuf), &rc, DT_CENTER);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, g_hdcMem, 0, 0, SRCCOPY);
    EndPaint(hwnd, &ps);
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    static DWORD dwStopAts[2] = {1'000'000, 2'000'000};
    static HANDLE hThreads[2] = {NULL, NULL};

    switch (id) {
    case ID_EXPT_RUN:
    {
        // 防止多次进入
        if (hThreads[0] == NULL && hThreads[1] == NULL) {
            g_bShouldExit_shared = FALSE;
            hThreads[0] = CreateThread(NULL, 0, CountNumber, &dwStopAts[0], 0, NULL);
            hThreads[1] = CreateThread(NULL, 0, CountNumber, &dwStopAts[1], 0, NULL);
        }
        else {
            for (int i = 0; i < 2; i++)
                ResumeThread(hThreads[i]);
        }
        break;
    }
    case ID_EXPT_PAUSE:
    {
        for (int i = 0; i < 2; i++)
            SuspendThread(hThreads[i]);
        break;
    }
    case ID_EXPT_STOP:
    {
        g_bShouldExit_shared = TRUE;
        if (hThreads[0] != NULL || hThreads[1] != NULL) {
            for (int i = 0; i < 2; i++)
                ResumeThread(hThreads[i]);

            WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
            hThreads[0] = NULL;
            hThreads[1] = NULL;
        }

        g_dwTotal_shared = 0;
        InvalidateRect(g_hwndMain, NULL, FALSE);
        break;
    }
    case ID_FILE_EXIT:
    {
        SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_EXPT_STOP, 0), 0);
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
    case WM_USER:
        InvalidateRect(hWnd, NULL, FALSE);
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

DWORD WINAPI CountNumber(LPVOID pArgs)
{
    DWORD *pdwStopAt = (DWORD*)pArgs;
    DWORD dwThreadId = GetCurrentThreadId();

    for (DWORD i = 0; i < *pdwStopAt; i++) {
        // g_dwTotal_shared++;
        InterlockedIncrement(&g_dwTotal_shared);
        
        InvalidateRect(g_hwndMain, NULL, FALSE);
        if (g_bShouldExit_shared) break;
    }

    PostMessage(g_hwndMain, WM_USER, 0, 0);

    return *pdwStopAt;
}