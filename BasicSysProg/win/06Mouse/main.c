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

	HWND hWndMain = CreateWindow(wszTitle, L"ЪѓБъ", WS_OVERLAPPEDWINDOW, (cxScreen - width)/2, (cyScreen - height)/2, width, height, NULL, NULL, hInstance, NULL);
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

	switch (uMsg) {
	case WM_CREATE:
	{
        
        pModel = malloc(sizeof(Model));
        ModelInit(pModel);

        pView = malloc(sizeof(View));
        ViewInit(pView, pModel, hWnd);

		ShowWindow(hWnd, SW_SHOW);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;

		hdc = BeginPaint(hWnd, &ps);
        pView->pAPI->OnPaint(pView, hdc);
		EndPaint(hWnd, &ps);
		break;
	}
    case WM_LBUTTONUP:
    {
        Point point;
        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);

        pModel->pAPI->Put(pModel, point);
        
        InvalidateRect(hWnd, NULL, FALSE);
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

		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}