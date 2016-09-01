#include "stdafx.h"
#include "resource.h"

#define WM_USER_BMP WM_USER + 6
HDC g_hdcMem = NULL;
SIZE g_szBmp;
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void LoadBMP(wchar_t*, HWND);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
    wchar_t wszTitle[] = L"Bitmap";

    WNDCLASS wcls;
    ZeroMemory(&wcls, sizeof(WNDCLASS));
    wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcls.lpfnWndProc = WindowProc;
    wcls.hInstance = hInstance;
    wcls.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
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

    HWND hWndMain = CreateWindow(wszTitle, L"位图", WS_OVERLAPPEDWINDOW, (cxScreen - width) / 2, (cyScreen - height) / 2, width, height, NULL, NULL, hInstance, NULL);
    if (!hWndMain) {
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
    ShowWindow(hwnd, SW_SHOW);
    return TRUE;
}

void OnDestroy(HWND hwnd)
{
    if (g_hdcMem != NULL) {
        HBITMAP hBmpOld = SelectBitmap(g_hdcMem, NULL);
        if (hBmpOld != NULL) DeleteBitmap(hBmpOld);
        DeleteDC(g_hdcMem);
    }
    PostQuitMessage(0);
}

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    RECT rc;
    HDC hdc = BeginPaint(hwnd, &ps);
    GetClientRect(hwnd, &rc);
    if (g_hdcMem != NULL) {
        SetStretchBltMode(hdc, HALFTONE);
        SetBrushOrgEx(hdc, 0, 0, NULL);
        StretchBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, g_hdcMem, 0, 0, g_szBmp.cx, g_szBmp.cy, SRCCOPY);
    }
    EndPaint(hwnd, &ps);
}

BOOL OnEraseBkgnd(HWND hwnd, HDC hdc)
{
    if (g_hdcMem == NULL) {
        RECT rc;
        GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
    }
    return TRUE;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id) {
    case ID_FILE_OPEN:
    {
        IFileDialog *pFileDialog = NULL;
        HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
        if (SUCCEEDED(hr)) {
            COMDLG_FILTERSPEC filter[2];
            filter[0].pszName = L"位图文件(*.bmp)";
            filter[0].pszSpec = L"*.bmp";
            filter[1].pszName = L"JPEG(*.jpg;*.jpeg)";
            filter[1].pszSpec = L"*.jpg;*.jpeg";

            hr = pFileDialog->SetFileTypes(2, filter);
            hr = pFileDialog->SetFileTypeIndex(1);
            hr = pFileDialog->Show(hwnd);
            if (SUCCEEDED(hr)) {
                IShellItem *psi = NULL;
                hr = pFileDialog->GetResult(&psi);
                if (SUCCEEDED(hr)) {
                    PWSTR pwszFilePath = NULL;
                    hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pwszFilePath);
                    if (SUCCEEDED(hr)) {
                        SendMessage(hwnd, WM_USER_BMP, 0, (LPARAM)pwszFilePath);
                        CoTaskMemFree(pwszFilePath);
                    }
                    psi->Release();
                }
            }
        }
        pFileDialog->Release();
        
        break;
    }
    case ID_FILE_EXIT:
        PostQuitMessage(0);
        break;
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
        HANDLE_MSG(hWnd, WM_ERASEBKGND, OnEraseBkgnd);
    case WM_USER_BMP:
    {
        wchar_t *pwszFile = (wchar_t*)lParam;
        LoadBMP(pwszFile, hWnd);
        break;
    }
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0L;
}

void LoadBMP(wchar_t *pwszBmp, HWND hwnd)
{
    // read the bmp content
    HBITMAP hBmp = (HBITMAP)LoadImage(NULL, pwszBmp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    BITMAP bmp;
    GetObject(hBmp, sizeof(BITMAP), &bmp);
    g_szBmp.cx = bmp.bmWidth;
    g_szBmp.cy = bmp.bmHeight;

    if (g_hdcMem == NULL) {
        HDC hdc = GetDC(hwnd);
        g_hdcMem = CreateCompatibleDC(hdc);
        ReleaseDC(hwnd, hdc);
    }
    
    HBITMAP hBmpOld = SelectBitmap(g_hdcMem, hBmp);
    if (hBmpOld != NULL) DeleteObject(hBmpOld);

    InvalidateRect(hwnd, NULL, TRUE);
}