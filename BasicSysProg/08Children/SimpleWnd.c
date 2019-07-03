#include "stdafx.h"
#include "SimpleWnd.h"

LRESULT CALLBACK SimpleWndProc(HWND, UINT, WPARAM, LPARAM);

void SimpleWndCreate(SimpleWnd *pThis, HWND hParent, const RECT *pRect, const wchar_t *pwszName)
{
    pThis->hwnd = CreateWindow(pThis->pAPI->pwszClsName, pwszName, WS_CHILD | WS_VISIBLE,
        pRect->left, pRect->top, pRect->right - pRect->left, pRect->bottom - pRect->top, hParent, NULL, NULL, NULL);
}

SimpleWnd* SimpleWndInit(SimpleWnd *pThis)
{
    static SimpleWndFunctions s_fns =
    {
        .pwszClsName = L"SimpleWnd",
        .Create = SimpleWndCreate,
    };

    ZeroMemory(pThis, sizeof(SimpleWnd));
    pThis->pAPI = &s_fns;

    WNDCLASS cls;
    ZeroMemory(&cls, sizeof(WNDCLASS));
    cls.style = CS_HREDRAW | CS_VREDRAW;
    cls.lpfnWndProc = SimpleWndProc;
    cls.hbrBackground = GetStockBrush(WHITE_BRUSH);
    cls.hInstance = GetModuleHandle(NULL);
    cls.hCursor = LoadCursor(NULL, IDC_ARROW);
    cls.lpszClassName = s_fns.pwszClsName;

    RegisterClass(&cls);

    return pThis;
}

void SimpleWndOnPaint(HWND hwnd)
{
    wchar_t *pwszText = L"I'm a simple window!";

    RECT rc;
    GetClientRect(hwnd, &rc);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, pwszText, (int)wcslen(pwszText), &rc, DT_CENTER);
    EndPaint(hwnd, &ps);
}

LRESULT CALLBACK SimpleWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        HANDLE_MSG(hWnd, WM_PAINT, SimpleWndOnPaint);
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0L;
}