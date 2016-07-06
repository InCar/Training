#include "stdafx.h"
#include "resource.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
    wchar_t wszTitle[] = L"Menu";

    WNDCLASS wcls;
    ZeroMemory(&wcls, sizeof(WNDCLASS));
    wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcls.lpfnWndProc = WindowProc;
    wcls.hInstance = hInstance;
    wcls.hbrBackground = GetStockObject(LTGRAY_BRUSH);
    wcls.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONAPP));
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

    HWND hWndMain = CreateWindow(wszTitle, L"¶Ô»°¿ò", WS_OVERLAPPEDWINDOW, (cxScreen - width) / 2, (cyScreen - height) / 2, width, height, NULL, NULL, hInstance, NULL);
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

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
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

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id) {
    case ID_FILE_EXIT:
        PostQuitMessage(0);
        break;
    case ID_FILE_OPEN:
    {
        wchar_t wcsBuf[MAX_PATH];
        ZeroMemory(wcsBuf, sizeof(wcsBuf));

        OPENFILENAME ofn;
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hwnd;
        ofn.hInstance = GetModuleHandle(NULL);
        ofn.lpstrFile = wcsBuf;
        ofn.nMaxFile  = MAX_PATH;
        ofn.Flags = OFN_FILEMUSTEXIST;
        if (GetOpenFileName(&ofn)) {
            OutputDebugString(wcsBuf);
        }
        break;
    }   
    case ID_HELP_ABOUT:
    {
        wchar_t wcsBuf[64] = L"Hello";
        INT_PTR nResult = DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ABOUT), hwnd, AboutDlgProc, (LPARAM)wcsBuf);
        
        OutputDebugString(wcsBuf);
        OutputDebugString(L"\r\n");
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
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0L;
}

INT_PTR CALLBACK AboutDlgProc(HWND hDialog, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static wchar_t *pwszBuf = NULL;
    switch (uMsg)
    {
    case WM_INITDIALOG:
        pwszBuf = (wchar_t*)lParam;
        SetDlgItemText(hDialog, IDC_EDIT_NAME, pwszBuf);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            GetDlgItemText(hDialog, IDC_EDIT_NAME, pwszBuf, 64);
        case IDCANCEL:
            EndDialog(hDialog, 5);
            return TRUE;
        default:
            break;
        }
        break;
    }
    return FALSE;
}