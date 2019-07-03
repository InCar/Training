#include "stdafx.h"
#include "resource.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
    wchar_t wszTitle[] = L"ClipBoard";

    WNDCLASS wcls;
    ZeroMemory(&wcls, sizeof(WNDCLASS));
    wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcls.lpfnWndProc = WindowProc;
    wcls.hInstance = hInstance;
    wcls.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wcls.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONAPP));
    wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcls.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
    wcls.lpszClassName = wszTitle;

    if (!RegisterClass(&wcls)) {
        MessageBox(NULL, L"Register class failed", L"Error", MB_OK);
        return GetLastError();
    }

    int cxScreen = GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);
    int width = cxScreen > 800 ? 800 : cxScreen;
    int height = cyScreen > 600 ? 600 : cyScreen;

    HWND hWndMain = CreateWindow(wszTitle, L"¼ôÌù°å", WS_OVERLAPPEDWINDOW, (cxScreen - width) / 2, (cyScreen - height) / 2, width, height, NULL, NULL, hInstance, NULL);
    if (!hWndMain) {
        MessageBox(NULL, L"Create window failed", L"Error", MB_OK);
        return GetLastError();
    }

    HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(hWndMain, hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    DestroyAcceleratorTable(hAccel);

    return (int)msg.wParam;
}

struct Model
{
    // ×Ö·û»º³åÇø³¤¶È
    unsigned int len;
    // ×Ö·û»º³åÇø,´æ·Å¼üÈëµÄ×Ö·ûÐòÁÐ
    wchar_t *pwcsBuffer;
    // ×ÖÌå
    HFONT font;
} g_model;

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    g_model.len = 4096;
    g_model.pwcsBuffer = malloc(sizeof(wchar_t) * g_model.len);
    StringCchCopy(g_model.pwcsBuffer, g_model.len, L"¾ÍÐ÷");

    LOGFONT lf;
    ZeroMemory(&lf, sizeof(LOGFONT));
    wcscpy_s(lf.lfFaceName, sizeof(lf.lfFaceName) / sizeof(wchar_t), L"ËÎÌå");
    g_model.font = CreateFontIndirect(&lf);

    ShowWindow(hwnd, SW_SHOW);
    return TRUE;
}

void OnDestroy(HWND hwnd)
{
    free(g_model.pwcsBuffer);
    DeleteFont(g_model.font);

    PostQuitMessage(0);
}

void OnPaint(HWND hwnd)
{
    RECT rcClient;
    GetClientRect(hwnd, &rcClient);
    rcClient.left += 4;
    rcClient.top += 4;
    rcClient.right -= 4;
    rcClient.bottom -= 4;

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    SelectFont(hdc, g_model.font);
    SetBkMode(hdc, TRANSPARENT);

    DrawText(hdc, g_model.pwcsBuffer, -1, &rcClient, DT_LEFT);
    EndPaint(hwnd, &ps);
}

void OnChar(HWND hwnd, TCHAR ch, int cRepeat)
{
    switch (ch)
    {
    case L'\b':
    {
        unsigned int count = (unsigned int)wcslen(g_model.pwcsBuffer);
        if (count > 0) {
            g_model.pwcsBuffer[count - 1] = 0;
        }
        break;
    }
    default:
    {
        unsigned int count = (unsigned int)wcslen(g_model.pwcsBuffer);
        if (count >= g_model.len - 1) {
            // no more space, expand
            int szExpand = g_model.len + 4096;
            wchar_t *pNewBuffer = realloc(g_model.pwcsBuffer, sizeof(wchar_t)*szExpand);
            if (pNewBuffer == NULL) return; // failed to realloc
            g_model.pwcsBuffer = pNewBuffer;
            g_model.len = szExpand;
        }

        g_model.pwcsBuffer[count] = ch;
        g_model.pwcsBuffer[count + 1] = 0;
        break;
    }
    }

    InvalidateRect(hwnd, NULL, TRUE);
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id) {
    case ID_EDIT_COPY:
    {
        unsigned int count = (unsigned int)wcslen(g_model.pwcsBuffer);
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, sizeof(wchar_t)*(count+1));
        LPVOID pBuffer = GlobalLock(hMem);
        StringCchCopy(pBuffer, sizeof(wchar_t)*(count + 1), g_model.pwcsBuffer);
        GlobalUnlock(hMem);

        OpenClipboard(hwnd);
        EmptyClipboard();
        SetClipboardData(CF_UNICODETEXT, hMem);
        CloseClipboard();
        break;
    }
    case ID_EDIT_PASTE:
    {
        OpenClipboard(hwnd);
        HANDLE hMem = GetClipboardData(CF_UNICODETEXT);
        if (hMem != NULL) {
            wchar_t *pwcsBuf = GlobalLock(hMem);
            unsigned int count = (unsigned int)wcslen(pwcsBuf);
            if (count > g_model.len - 1) {
                // Need expand
                wchar_t *pNewBuffer = realloc(g_model.pwcsBuffer, sizeof(wchar_t)*(count+1));
                if (pNewBuffer != NULL) {
                    g_model.pwcsBuffer = pNewBuffer;
                    g_model.len = count + 1;
                }
            }

            StringCchCopy(g_model.pwcsBuffer, sizeof(wchar_t)*(g_model.len + 1), pwcsBuf);
            InvalidateRect(hwnd, NULL, TRUE);

            GlobalUnlock(hMem);
        }
        CloseClipboard();
        break;
    }
    case ID_FILE_QUIT:
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
        HANDLE_MSG(hWnd, WM_CHAR, OnChar);
        HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0L;
}