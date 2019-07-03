#include "stdafx.h"
#include "resource.h"
#include "chat.h"

#define MARGIN 4

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ConfigDialogProc(HWND, UINT, WPARAM, LPARAM);
void ShowLastError(HWND, DWORD);

Chat g_chat;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
    wchar_t wszTitle[] = L"Network";

    WNDCLASS wcls;
    ZeroMemory(&wcls, sizeof(WNDCLASS));
    wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcls.lpfnWndProc = WindowProc;
    wcls.hInstance = hInstance;
    wcls.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wcls.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_APP));
    wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcls.lpszClassName = wszTitle;

    if (!RegisterClass(&wcls)) {
        MessageBox(NULL, L"Register class failed", L"Error", MB_OK);
        return GetLastError();
    }

    int cxScreen = GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);
    int width = cxScreen > 800 ? 800 : (int)(cxScreen*0.75f);
    int height = cyScreen > 600 ? 600 : (int)(cyScreen*0.75f);

    ChatStartup(&g_chat);

    HWND hwndMain = CreateWindow(wszTitle, L"加密通信", WS_OVERLAPPEDWINDOW, (cxScreen - width) / 2, (cyScreen - height) / 2, width, height, NULL, NULL, hInstance, NULL);
    if (!hwndMain) {
        MessageBox(NULL, L"Create window failed", L"Error", MB_OK);
        return GetLastError();
    }
    UpdateWindow(hwndMain);

    g_chat.hwndMain = hwndMain;
    HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));

    BOOL bExit = FALSE;
    MSG msg;
    HANDLE evs[] = { g_chat.ev15673, g_chat.ev15683 };
    do {
        int nEV = sizeof(evs) / sizeof(HANDLE);
        DWORD dwWait = MsgWaitForMultipleObjects(nEV, evs, FALSE, INFINITE, QS_ALLEVENTS&(~QS_RAWINPUT));
        
        if ((int)dwWait >= nEV) {
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    bExit = TRUE;
                    break;
                }
                else if (msg.message == WM_RBUTTONUP) {
                    POINT point = {
                        .x = GET_X_LPARAM(msg.lParam),
                        .y = GET_Y_LPARAM(msg.lParam)
                    };
                    ClientToScreen(msg.hwnd, &point);
                    HMENU hPop = CreatePopupMenu();
                    AppendMenu(hPop, MF_STRING, ID_OPTIONS_CONFIG, L"设置");
                    TrackPopupMenu(hPop, TPM_LEFTALIGN, point.x, point.y, 0, hwndMain, NULL);
                    DestroyMenu(hPop);
                }
                else if (!TranslateAccelerator(hwndMain, hAccel, &msg)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            if (bExit) break; // Quit 
        }
        else if(dwWait == WAIT_OBJECT_0 ){
            // 收到通信事件
            PostMessage(hwndMain, WM_RECV15673, 0, 0);
            ResetEvent(g_chat.ev15673);
        }
        else if (dwWait == WAIT_OBJECT_0 + 1) {
            // TCP 15683 CONNECT or READ
            ChatAcceptOrRead();
            ResetEvent(g_chat.ev15683);
        }
    } while (TRUE);

    DestroyAcceleratorTable(hAccel);
    ChatShutdown(&g_chat);

    return (int)msg.wParam;
}

void CalcChildrenSize(int cx, int cy, RECT *pRect, int nCount)
{
    // 计算子窗口的尺寸 hwndSend, hwndInput, hwndList, hwndHistory
    if (nCount < 5) return; // 数目不对头

    RECT *prcSend = pRect+0;
    prcSend->bottom = cy - MARGIN;
    prcSend->top = cy - 80;
    prcSend->right = cx - MARGIN;
    prcSend->left = cx - 80;

    RECT *prcInput = pRect + 1;
    prcInput->left = MARGIN;
    prcInput->right = prcSend->left - MARGIN;
    prcInput->bottom = prcSend->bottom;
    prcInput->top = prcSend->top;

    RECT *prcList = pRect + 2;
    prcList->top = MARGIN;
    prcList->bottom = prcInput->top - 30 - MARGIN*2;
    prcList->left = prcSend->right - 180;
    prcList->right = prcSend->right;

    RECT *prcRefresh = pRect + 3;
    prcRefresh->top = prcList->bottom + MARGIN / 2;
    prcRefresh->bottom = prcInput->top - MARGIN;
    prcRefresh->left = prcList->left;
    prcRefresh->right = prcList->right;

    RECT *prcHistory = pRect + 4;
    prcHistory->top = MARGIN;
    prcHistory->bottom = prcInput->top - MARGIN;
    prcHistory->left = prcInput->left;
    prcHistory->right = prcList->left - MARGIN;
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    // prepare the font
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(LOGFONT));
    lf.lfHeight = 12;
    lf.lfWeight = FW_MEDIUM;
    wcscpy_s(lf.lfFaceName, 32, L"宋体");
    g_chat.hfontText = CreateFontIndirect(&lf);

    RECT rcChildren[5];
    CalcChildrenSize(rc.right, rc.bottom, (RECT*)&rcChildren, 5);

    RECT *prcSend = rcChildren + 0;
    g_chat.hwndSend = CreateWindow(L"Button", L"发送[Alt+S]", WS_CHILD | WS_VISIBLE | WS_BORDER,
        prcSend->left, prcSend->top, prcSend->right - prcSend->left, prcSend->bottom - prcSend->top, hwnd, (HMENU)ID_MSG_SEND, NULL, NULL);
    SendMessage(g_chat.hwndSend, WM_SETFONT, (WPARAM)g_chat.hfontText, FALSE);

    RECT *prcInput = rcChildren + 1;
    g_chat.hwndInput = CreateWindow(L"Edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
        prcInput->left, prcInput->top, prcInput->right - prcInput->left, prcInput->bottom - prcInput->top, hwnd, NULL, NULL, NULL);
    SendMessage(g_chat.hwndInput, WM_SETFONT, (WPARAM)g_chat.hfontText, FALSE);
    SendMessage(g_chat.hwndInput, EM_LIMITTEXT, MAX_MESSAGE_LEN, 0);
    SetFocus(g_chat.hwndInput);

    RECT *prcList = rcChildren + 2;
    g_chat.hwndList = CreateWindow(L"ListBox", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_HASSTRINGS | LBS_SORT | LBS_NOINTEGRALHEIGHT,
        prcList->left, prcList->top, prcList->right - prcList->left, prcList->bottom - prcList->top, hwnd, NULL, NULL, NULL);
    SendMessage(g_chat.hwndList, WM_SETFONT, (WPARAM)g_chat.hfontText, FALSE);

    RECT *prcRefresh = rcChildren + 3;
    g_chat.hwndRefresh = CreateWindow(L"Button", L"刷新[Alt+R]", WS_CHILD | WS_VISIBLE | WS_BORDER,
        prcSend->left, prcSend->top, prcSend->right - prcSend->left, prcSend->bottom - prcSend->top, hwnd, (HMENU)ID_MSG_REFRESH, NULL, NULL);
    SendMessage(g_chat.hwndRefresh, WM_SETFONT, (WPARAM)g_chat.hfontText, FALSE);

    RECT *prcHistory = rcChildren + 4; 
    g_chat.hwndHistory = CreateWindow(L"ListBox", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT,
        prcHistory->left, prcHistory->top, prcHistory->right - prcHistory->left, prcHistory->bottom - prcHistory->top, hwnd, NULL, NULL, NULL);
    SendMessage(g_chat.hwndHistory, WM_SETFONT, (WPARAM)g_chat.hfontText, FALSE);

    wchar_t buf[1024];
    StringCchPrintf(buf, 1024, L"加密通信 - %s", g_chat.wszNick);
    SetWindowText(hwnd, buf);

    UpdateWindow(hwnd);
    ShowWindow(hwnd, SW_SHOW);

    // 启动监听
    PostMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_MSG_REFRESH, 0), 0);

    return TRUE;
}

void OnDestroy(HWND hwnd)
{
    DeleteFont(g_chat.hfontText);

    PostQuitMessage(0);
}

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    EndPaint(hwnd, &ps);
}

void OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    RECT rc[5];
    CalcChildrenSize(cx, cy, (RECT*)&rc, sizeof(rc)/sizeof(RECT));
    
    HWND hwndChildren[] = { g_chat.hwndSend, g_chat.hwndInput, g_chat.hwndList, g_chat.hwndRefresh, g_chat.hwndHistory };
    for (int i = 0; i < sizeof(rc) / sizeof(RECT); i++) {
        SetWindowPos(hwndChildren[i], NULL, rc[i].left, rc[i].top, rc[i].right - rc[i].left, rc[i].bottom - rc[i].top, SWP_NOZORDER);
    }
}

LRESULT OnSizing(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    RECT *pRC = (RECT*)lParam;
    if (pRC->right - pRC->left < 300) {
        if (wParam == WMSZ_LEFT || wParam == WMSZ_TOPLEFT || wParam == WMSZ_BOTTOMLEFT)
            pRC->left = pRC->right - 300;
        else
            pRC->right = pRC->left + 300;
    }

    if (pRC->bottom - pRC->top < 200) {
        if (wParam == WMSZ_TOP || wParam == WMSZ_TOPLEFT || wParam == WMSZ_TOPRIGHT)
            pRC->top = pRC->bottom - 200;
        else
            pRC->bottom = pRC->top + 200;
    }
    
    return TRUE;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id) {
    case ID_FILE_EXIT:
    {
        PostQuitMessage(0);
        break;
    }
    case ID_OPTIONS_CONFIG:
    {
        DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG_CONFIG), hwnd, ConfigDialogProc);
        break;
    }
    case ID_MSG_SEND:
    {
        wchar_t buf[MAX_MESSAGE_LEN], buf2[MAX_MESSAGE_LEN];
        if (Edit_GetTextLength(g_chat.hwndInput) > 0) {
            Edit_GetText(g_chat.hwndInput, buf, MAX_MESSAGE_LEN);
            
            // 目标
            int nSelected = ListBox_GetCurSel(g_chat.hwndList);
            if (nSelected != LB_ERR) {
                wchar_t target[64];
                ListBox_GetText(g_chat.hwndList, nSelected, target);
                // 解出IP
                wchar_t *pL = wcschr(target, L'[');
                wchar_t *pR = wcschr(target, L']');
                if (pL && pR) {
                    wchar_t *pXIP = pL + 1;
                    *pR = L'\0';
                    ChatSend(pXIP, buf);

                    // 清理发送输入栏
                    StringCchPrintf(buf2, MAX_MESSAGE_LEN, L"%s:%s", g_chat.wszNick, buf);
                    ListBox_AddString(g_chat.hwndHistory, buf2);
                    Edit_SetText(g_chat.hwndInput, L"");
                }
            }
                          
        }
        break;
    }
    case ID_MSG_MYID:
    {
        // 报告自己身份
        BroadcastPack pack;
        memcpy_s(pack.tag, sizeof(pack.tag), "IC2016IC", 8);
        memcpy_s(pack.cmd, sizeof(pack.cmd), "MYID", 4);

        wcscpy_s((wchar_t*)pack.buf, sizeof(pack.buf) / sizeof(WORD), g_chat.wszNick);
        pack.len = (unsigned char)(wcslen(g_chat.wszNick) + 1)*sizeof(wchar_t);

        ChatBroadcast((char*)&pack, 13 + pack.len);
        break;
    }
    case ID_MSG_REFRESH:
    {
        // 清空列表
        int nLeft = 0;
        do {
            nLeft = (int)SendMessage(g_chat.hwndList, LB_DELETESTRING, 0, 0);
        } while (nLeft > 0);

        // 广播全体报告
        BroadcastPack pack;
        memcpy_s(pack.tag, sizeof(pack.tag), "IC2016IC", 8);
        memcpy_s(pack.cmd, sizeof(pack.cmd), "REPT", 4);
        pack.len = 0;

        ChatBroadcast((char*)&pack, 13 + pack.len);
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
        HANDLE_MSG(hWnd, WM_SIZE, OnSize);
        HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
    case WM_SIZING:
        return OnSizing(hWnd, wParam, lParam);
        break;
    case WM_RECV15673:
        OnBroadcast();
        break;
    case WM_UPDATE_ID:
    {
        // 只保留IP
        wchar_t *pColon = wcschr((wchar_t*)wParam, L':');
        if (pColon) *pColon = L'\0';

        wchar_t buf[64];
        StringCchPrintf(buf, 64, L"%s[%s]", lParam, wParam);

        if(SendMessage(g_chat.hwndList, LB_FINDSTRING, 0, (LPARAM)buf) == LB_ERR)
            SendMessage(g_chat.hwndList, LB_ADDSTRING, 0, (LPARAM)buf);
        break;
    }
    case WM_INCOMING_MSG:
    {
        wchar_t buf[128];

        // 只保留IP
        wchar_t *pColon = wcschr((wchar_t*)wParam, L':');
        if (pColon) *pColon = L'\0';

        StringCchPrintf(buf, 128, L"%s:%s", wParam, lParam);
        ListBox_AddString(g_chat.hwndHistory, buf);
        break;
    }
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0L;
}

INT_PTR CALLBACK ConfigDialogProc(HWND hDialog, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        SetDlgItemText(hDialog, IDC_EDIT_NICK, g_chat.wszNick);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            GetDlgItemText(hDialog, IDC_EDIT_NICK, g_chat.wszNick, MAX_NICK_LEN);

            wchar_t buf[1024];
            StringCchPrintf(buf, 1024, L"加密通信 - %s", g_chat.wszNick);
            SetWindowText(GetParent(hDialog), buf);

        case IDCANCEL:
            EndDialog(hDialog, 0);
            return TRUE;
        default:
            break;
        }
        break;
    }
    return FALSE;
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