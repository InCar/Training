#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
	wchar_t wszTitle[] = L"Text Scroll";

	WNDCLASS wcls;
	ZeroMemory(&wcls, sizeof(WNDCLASS));
	wcls.style = CS_HREDRAW | CS_VREDRAW;
	wcls.lpfnWndProc = WindowProc;
	wcls.hInstance = hInstance;
	wcls.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcls.hIcon = LoadIcon(NULL, IDI_SHIELD);
	wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcls.lpszClassName = wszTitle;


	if (!RegisterClass(&wcls)) {
		MessageBox(NULL, L"Register class failed", L"Error", MB_OK);
		return GetLastError();
	}

	HWND hWndMain = CreateWindow(wszTitle, wszTitle, WS_OVERLAPPEDWINDOW|WS_VSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
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
	static int s_nDeltaY = 0, s_nCharAvg = 0;
	static int s_nMargin = 4;

	static int s_cxScreen = 0, s_cyScreen = 0, s_cxVScroll = 0;

	SCROLLINFO si;

	switch (uMsg) {
	case WM_CREATE:
	{
		TEXTMETRIC tm;

		HDC hdc = GetDC(hWnd);
		GetTextMetrics(hdc, &tm);
		s_nDeltaY = tm.tmExternalLeading + tm.tmHeight + s_nMargin;
		s_nCharAvg = tm.tmAveCharWidth;
		ReleaseDC(hWnd, hdc);

		s_cxScreen = GetSystemMetrics(SM_CXSCREEN);
		s_cyScreen = GetSystemMetrics(SM_CYSCREEN);
		s_cxVScroll = GetSystemMetrics(SM_CXVSCROLL);

		ShowWindow(hWnd, SW_SHOW);
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		RECT rect;
		HDC hdc;

		int nYPos = GetScrollPos(hWnd, SB_VERT);

		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		HFONT hFont = GetStockObject(OEM_FIXED_FONT);
		HGDIOBJ hOldFont = SelectObject(hdc, hFont);
		// DrawText(hdc, L"你好,消息循环", -1, &rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

		SetBkMode(hdc, OPAQUE);
		
		wchar_t wszText[] = L"文本输出示例!";
		TextOut(hdc, s_nMargin, s_nMargin - nYPos, wszText, (int)wcslen(wszText));
		TextOut(hdc, s_nMargin, s_nMargin + s_nDeltaY - nYPos, wszText, (int)wcslen(wszText));

		wchar_t wszBuf[128];
		wsprintf(wszBuf, L"屏幕宽度:%8d", s_cxScreen);
		TextOut(hdc, s_nMargin, s_nMargin + s_nDeltaY * 2 - nYPos, wszBuf, lstrlen(wszBuf));
		wsprintf(wszBuf, L"屏幕高度:%8d", s_cyScreen);
		TextOut(hdc, s_nMargin, s_nMargin + s_nDeltaY * 3 - nYPos, wszBuf, lstrlen(wszBuf));
		
		wsprintf(wszBuf, L"垂直滚动条宽度:%8d", s_cxVScroll);
		TextOut(hdc, s_nMargin, s_nMargin + s_nDeltaY * 5 - nYPos, wszBuf, lstrlen(wszBuf));
		
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);
		wsprintf(wszBuf, L"我是最底部的一行");
		TextOut(hdc, s_nMargin, rcClient.bottom - s_nDeltaY, wszBuf, lstrlen(wszBuf));

		SelectObject(hdc, hOldFont);

		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_SIZE:
	{
		int cx = LOWORD(lParam);
		int cy = HIWORD(lParam);

		wchar_t wszBuf[128];
		wsprintf(wszBuf, L"%4d x %4d", cx, cy);

		HDC hdc = GetDC(hWnd);
		TextOut(hdc, cx- s_nCharAvg * 11, cy - s_nDeltaY, wszBuf, lstrlen(wszBuf));
		ReleaseDC(hWnd, hdc);

		ZeroMemory(&si, sizeof(SCROLLINFO));
		si.cbSize = sizeof(SCROLLBARINFO);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 1000;
		si.nPage = cy;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

		return 0;
	}
	case WM_VSCROLL:
	{
		ZeroMemory(&si, sizeof(SCROLLINFO));
		si.cbSize = sizeof(SCROLLBARINFO);
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_VERT, &si);

		int nYPos = si.nPos;

		SetScrollPos(hWnd, SB_VERT, si.nTrackPos, TRUE);

		RECT rcClient;
		GetClientRect(hWnd, &rcClient);
		rcClient.bottom -= s_nDeltaY;
		ScrollWindowEx(hWnd, 0, nYPos-si.nTrackPos, &rcClient, &rcClient, NULL, NULL, SW_INVALIDATE| SW_ERASE);
		
		// GetClientRect(hWnd, &rcClient);
		// rcClient.top = rcClient.bottom - s_nDeltaY;
		// InvalidateRect(hWnd, &rcClient, FALSE);

		UpdateWindow(hWnd);

		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}