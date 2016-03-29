#include "stdafx.h"
#include "model.h"
#include "view.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
	wchar_t wszTitle[] = L"Mouse";

	WNDCLASS wcls;
	ZeroMemory(&wcls, sizeof(WNDCLASS));
	wcls.style = CS_HREDRAW | CS_VREDRAW;
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

	HWND hWndMain = CreateWindow(wszTitle, L"鼠标", WS_OVERLAPPEDWINDOW, (cxScreen - width)/2, (cyScreen - height)/2, width, height, NULL, NULL, hInstance, NULL);
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

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static Model *pModel = NULL;
    static View *pView = NULL;
    static HDC hdcMem;
    static HBITMAP hBmp;

	switch (uMsg) {
	case WM_CREATE:
	{
        
        pModel = malloc(sizeof(Model));
        ModelInit(pModel);

        pView = malloc(sizeof(View));
        ViewInit(pView, pModel, hWnd);

        // 最大可能的桌面尺寸
        int maxWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
        int maxHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

        // 双缓冲
        HDC hdc = GetDC(hWnd);
        hdcMem = CreateCompatibleDC(hdc);
        hBmp = CreateCompatibleBitmap(hdc, maxWidth, maxHeight);
        SelectObject(hdcMem, hBmp);
        ReleaseDC(hWnd, hdc);

		ShowWindow(hWnd, SW_SHOW);
		break;
	}
    case WM_ERASEBKGND:
    {
        break;
    }
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
        RECT rect;

		hdc = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &rect);
        // 绘制在不可见的缓冲区上
        pView->pAPI->OnPaint(pView, hdcMem);
        // 复制到可见的前台缓冲区
        BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
		break;
	}
    case WM_LBUTTONUP:
    {
        Point point;
        point.x = GET_X_LPARAM(lParam);
        point.y = GET_Y_LPARAM(lParam);

        ReleaseCapture();
        
        if (!pView->pAPI->DragEnd(pView, point)) {
            pModel->pAPI->Put(pModel, point);
        }

        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        Point point;
        point.x = GET_X_LPARAM(lParam);
        point.y = GET_Y_LPARAM(lParam);

        SetCapture(hWnd);

        pView->pAPI->DragStart(pView, point);
        break;
    }
    case WM_MOUSEMOVE:
    {
        Point point;
        point.x = GET_X_LPARAM(lParam);
        point.y = GET_Y_LPARAM(lParam);

        if (wParam & MK_LBUTTON) {
            pView->pAPI->Dragging(pView, point);
        }
        else {
            pView->pAPI->Hover(pView, point);
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_KEYUP:
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            pModel->pAPI->Clear(pModel);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        default:
            break;
        }
        break;
    }
	case WM_DESTROY:
	{
        pView->pAPI->Close(pView);

        free(pModel);
        free(pView);

        DeleteDC(hdcMem);
        DeleteObject(hBmp);

		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}