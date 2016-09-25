#include "stdafx.h"
#include "resource.h"

HANDLE g_hFile = INVALID_HANDLE_VALUE;
BYTE *g_pBuf = NULL;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void ShowLastError(HWND, DWORD);
void ReadFileAsHex();
void ReleaseFile();
void EnableSaveAndClose(HWND, BOOL);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
    wchar_t wszTitle[] = L"File";

    WNDCLASS wcls;
    ZeroMemory(&wcls, sizeof(WNDCLASS));
    wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcls.lpfnWndProc = WindowProc;
    wcls.hInstance = hInstance;
    wcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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

    HWND hWndMain = CreateWindow(wszTitle, L"文件", WS_OVERLAPPEDWINDOW, (cxScreen - width) / 2, (cyScreen - height) / 2, width, height, NULL, NULL, hInstance, NULL);
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
    EnableSaveAndClose(hwnd, FALSE);
    return TRUE;
}

void OnDestroy(HWND hwnd)
{
    ReleaseFile();
    PostQuitMessage(0);
}

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    RECT rc;
    TEXTMETRIC tm;
    HDC hdc = BeginPaint(hwnd, &ps);
    GetTextMetrics(hdc, &tm);
    GetClientRect(hwnd, &rc);

    if (g_pBuf != NULL) {
        unsigned int size = (unsigned int)_msize(g_pBuf);
        wchar_t wszBuf[4];
        for (unsigned int i = 0; i < size; i++) {
            StringCchPrintf(wszBuf, 4, L"%02X", g_pBuf[i]);
            int x = (i % 16) * tm.tmMaxCharWidth * 2 + 4 + tm.tmMaxCharWidth*2*((i % 16)<8?0:1);
            int y = (i / 16)*(tm.tmExternalLeading + tm.tmHeight + 4);
            if(y <= rc.bottom)
                TextOut(hdc, x, y, wszBuf, 2);
        }
    }


    EndPaint(hwnd, &ps);
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id) {
    case ID_FILE_OPEN:
    {
        wchar_t wcsFile[MAX_PATH] = L"\0";

        OPENFILENAME ofn;
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFilter = L"文本文件(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0\0";
        ofn.lpstrFile = wcsFile;
        ofn.nMaxFile = MAX_PATH;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_CREATEPROMPT|OFN_HIDEREADONLY;

        if (GetOpenFileName(&ofn)) {
            // Close the file that opened before
            ReleaseFile();
            
            DWORD dwOpenFlag = 0;
            if (PathFileExists(ofn.lpstrFile))
                dwOpenFlag = OPEN_EXISTING;
            else
                dwOpenFlag = CREATE_NEW;
            g_hFile = CreateFile(ofn.lpstrFile, GENERIC_ALL, 0, NULL, dwOpenFlag, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
            if (g_hFile == INVALID_HANDLE_VALUE) {
                ShowLastError(hwnd, GetLastError());
                return;
            }

            ReadFileAsHex();
            InvalidateRect(hwnd, NULL, TRUE);
            EnableSaveAndClose(hwnd, TRUE);
        }

        break;
    }
    case ID_FILE_SAVE:
    {
        if (g_hFile != INVALID_HANDLE_VALUE) {
            SetFilePointer(g_hFile, 0, NULL, FILE_END);
            char bufEnd[] = "\r\nEND";
            DWORD dwWritten = 0;
            WriteFile(g_hFile, bufEnd, (DWORD)strlen(bufEnd), &dwWritten, NULL);
            FlushFileBuffers(g_hFile);

            ReadFileAsHex();
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;
    }
    case ID_FILE_CLOSE:
    {
        ReleaseFile();
        InvalidateRect(hwnd, NULL, TRUE);
        EnableSaveAndClose(hwnd, FALSE);
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

void ReadFileAsHex()
{
    if (g_hFile != INVALID_HANDLE_VALUE) {
        DWORD dwSize = GetFileSize(g_hFile, NULL);
        DWORD dwRead = 0;
        g_pBuf = malloc(dwSize);
        SetFilePointer(g_hFile, 0, NULL, FILE_BEGIN);
        ReadFile(g_hFile, g_pBuf, dwSize, &dwRead, NULL);
    }
}

void ReleaseFile()
{
    if (g_pBuf != NULL) {
        free(g_pBuf);
        g_pBuf = NULL;
    }

    if (g_hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(g_hFile);
        g_hFile = INVALID_HANDLE_VALUE;
    }
}

void EnableSaveAndClose(HWND hwnd, BOOL bEnable)
{
    HMENU hMenu = GetMenu(hwnd);
    EnableMenuItem(hMenu, ID_FILE_SAVE, bEnable ? MF_ENABLED : MF_GRAYED);
    EnableMenuItem(hMenu, ID_FILE_CLOSE, bEnable ? MF_ENABLED : MF_GRAYED);
}