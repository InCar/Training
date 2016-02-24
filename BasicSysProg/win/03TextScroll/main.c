#include <Windows.h>
#include <stdio.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawLine(HDC hdc, wchar_t *pwcsBuffer, int nChar, int x, int y);

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
	static wchar_t *pwszBuffer = NULL;
	static wchar_t wszPoint[64] = L"\0";

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

		FILE *pSrc = NULL;
		if (_wfopen_s(&pSrc, L"main.c", L"r,ccs=UTF-8") == 0) {
			// make buffer to store file content
			fseek(pSrc, 0, SEEK_END);
			long lSize = ftell(pSrc);
			pwszBuffer = malloc(sizeof(wchar_t)*lSize);
			// read file content
			fseek(pSrc, 0, SEEK_SET);
			size_t nRead = fread(pwszBuffer, sizeof(wchar_t), lSize, pSrc);
			if (nRead < lSize) pwszBuffer[nRead] = 0;
			else pwszBuffer[nRead - 1] = 0;
			fclose(pSrc);
		}

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

		HRGN hClip = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom - s_nDeltaY - s_nMargin);
		SelectClipRgn(hdc, hClip);

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

		int nLine = 8;
		wchar_t *pLine = pwszBuffer, *pNextLine = NULL;
		if (pLine) {
			do {
				pNextLine = wcsstr(pLine, L"\n");
				if (pNextLine) {
					// TextOut(hdc, s_nMargin, s_nDeltaY * nLine - nYPos, pLine, (int)(pNextLine - pLine));
					*pNextLine = L'\0';
					DrawLine(hdc, pLine, (int)(pNextLine - pLine), s_nMargin, s_nDeltaY * nLine - nYPos);
					*pNextLine = L'\n';
					pLine = pNextLine + 1;
					nLine++;
				}
			} while (pNextLine);
			// The last line
			// TextOut(hdc, s_nMargin, s_nDeltaY * nLine - nYPos, pLine, lstrlen(pLine));
			DrawLine(hdc, pLine, lstrlen(pLine), s_nMargin, s_nDeltaY * nLine - nYPos);
		}

		RECT rcClient;
		GetClientRect(hWnd, &rcClient);
		SelectClipRgn(hdc, NULL);
		wsprintf(wszBuf, L"我是最底部的一行");
		TextOut(hdc, s_nMargin, rcClient.bottom - s_nDeltaY, wszBuf, lstrlen(wszBuf));

		SIZE szPoint;
		GetTextExtentPoint32(hdc, wszPoint, lstrlen(wszPoint), &szPoint); // 测量实际尺寸
		TextOut(hdc, rcClient.right - szPoint.cx - s_nMargin, rcClient.bottom - szPoint.cy - s_nMargin, wszPoint, lstrlen(wszPoint));

		SelectObject(hdc, hOldFont);

		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_SIZE:
	{
		int cx = LOWORD(lParam);
		int cy = HIWORD(lParam);

		wsprintf(wszPoint, L"%4d x %4d", cx, cy);

		ZeroMemory(&si, sizeof(SCROLLINFO));
		si.cbSize = sizeof(SCROLLBARINFO);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nMin = 0;
		si.nMax = 5000;
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
		rcClient.bottom -= (s_nDeltaY + s_nMargin);
		ScrollWindowEx(hWnd, 0, nYPos - si.nTrackPos, &rcClient, &rcClient, NULL, NULL, SW_INVALIDATE | SW_ERASE);

		UpdateWindow(hWnd);

		return 0;
	}
	case WM_DESTROY:
	{
		free(pwszBuffer);
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void DrawLine(HDC hdc, wchar_t *pwcsBuffer, int nChar, int x, int y)
{
	int x2 = x, y2 = y;

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);

	wchar_t *pwcsSegment = pwcsBuffer;
	wchar_t *pwcsNextSeg = wcsstr(pwcsSegment, L"\t");
	while (pwcsNextSeg) {
		TextOut(hdc, x2, y2, pwcsSegment, (int)(pwcsSegment - pwcsNextSeg));
		pwcsSegment = pwcsNextSeg + 1;
		x2 += (tm.tmAveCharWidth * 4);

		pwcsNextSeg = wcsstr(pwcsSegment, L"\t");
	}
	TextOut(hdc, x2, y2, pwcsSegment, lstrlen(pwcsSegment));
}