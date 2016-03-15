#include "stdafx.h"
#include "model.h"
#include "view.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
	wchar_t wszTitle[] = L"Keyboard";

	WNDCLASS wcls;
	ZeroMemory(&wcls, sizeof(WNDCLASS));
	wcls.style = CS_HREDRAW | CS_VREDRAW;
	wcls.lpfnWndProc = WindowProc;
	wcls.hInstance = hInstance;
	wcls.hbrBackground = CreateSolidBrush(RGB(0x00, 0x00, 0x00));
	wcls.hIcon = LoadIcon(NULL, IDI_SHIELD);
	wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcls.lpszClassName = wszTitle;


	if (!RegisterClass(&wcls)) {
		MessageBox(NULL, L"Register class failed", L"Error", MB_OK);
		return GetLastError();
	}

	HWND hWndMain = CreateWindow(wszTitle, L"¼üÅÌ", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	if (!hWndMain) {
		MessageBox(NULL, L"Create window failed", L"Error", MB_OK);
		return GetLastError();
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DeleteObject(wcls.hbrBackground);

	return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static Model model;
	static View view;

	switch (uMsg) {
	case WM_CREATE:
	{
		// ³õÊ¼»¯
		ModelInit(&model);
		ViewInit(&view, &model, hWnd);

		ShowWindow(hWnd, SW_SHOW);
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		RECT rect;
		HDC hdc;

		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);

		view.pAPI->OnPaint(&view, hdc);

		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_CHAR:
	{
		model.pAPI->Put(&model, (wchar_t)wParam);
		InvalidateRect(hWnd, NULL, FALSE);

		return 0;
	}
	case WM_KEYDOWN:
	{
		COLORREF color = 0;

		if (GetKeyState(VK_CONTROL) & 0x8000) {
			// CTRL is pressed down
			switch (wParam)
			{
			case 'R': color = RGB(0xff, 0x00, 0x00); break;
			case 'G': color = RGB(0x00, 0xff, 0x00); break;
			case 'B': color = RGB(0x00, 0x00, 0xff); break;
			}
			if (color) {
				view.pAPI->ChangeColor(&view, color);
				InvalidateRect(hWnd, NULL, FALSE);
			}
		}
		return 0;
	}
	case WM_SIZE:
	{
		RECT rect = {
			.left = 0,
			.top = 0,
			.right = LOWORD(lParam),
			.bottom = HIWORD(lParam)
		};


		return 0;
	}
	case WM_DESTROY:
	{
		model.pAPI->Clear(&model);
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}