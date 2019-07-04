#include "stdafx.h"
#include "resource.h"
#include "MainWnd.h"
#include "App.h"
#include "AdvCalc.h"

MainWnd::MainWnd()
{
}

MainWnd::~MainWnd()
{
}

ATOM MainWnd::Register()
{
	// 主窗口类型注册,隐藏了基类方法
	HINSTANCE hInstance = App::GetApp()->GetInstance();

	WNDCLASS wcls;
	ZeroMemory(&wcls, sizeof(WNDCLASS));
	wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcls.lpfnWndProc = XWnd::WindowProc;
	wcls.hInstance = hInstance;
	wcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcls.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONAPP)); // 图标
	wcls.lpszClassName = m_wstrClsName.c_str();

	return RegisterClass(&wcls);
}

BOOL MainWnd::Create()
{
	// 窗口尺寸和位置
	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYSCREEN);
	int width = cxScreen > 800 ? 800 : cxScreen;
	int height = cyScreen > 600 ? 600 : cyScreen;

	HINSTANCE hInstance = App::GetApp()->GetInstance();

	WNDCLASS cls;
	if (!GetClassInfo(hInstance, m_wstrClsName.c_str(), &cls)) Register();

	// 创建窗口
	wstring wstrTitle(L"Hello C++");
	m_hwnd = CreateWindow(m_wstrClsName.c_str(), wstrTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		(cxScreen - width) / 2, (cyScreen - height) / 2, width, height,
		NULL, NULL, hInstance, this);
	if (!m_hwnd) {
		MessageBox(NULL, L"Create window failed", L"Error", MB_OK);
		return FALSE;
	}

	// 第一次刷新窗口
	UpdateWindow(m_hwnd);
	// 自定义消息
	PostMessage(m_hwnd, WM_USER, 0, 0);

	return TRUE;
}

LRESULT MainWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		HANDLE_MSG(hWnd, WM_ERASEBKGND, OnEraseBkgnd);
		break;
	case WM_USER:
		return OnMyMsg(hWnd, wParam, lParam);
	default:
		return XWnd::WndProc(hWnd, uMsg, wParam, lParam);
	}
}

void MainWnd::OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	RECT rect;
	HDC hdc;

	hdc = BeginPaint(hwnd, &ps);

	// 绘制内容
	GetClientRect(hwnd, &rect);
	rect.right -= 24;
	rect.top = rect.bottom / 2 - 24;
	HGDIOBJ hOldFont = SelectObject(hdc, m_hfontSong);
	DrawText(hdc, m_wszBuf, -1, &rect, DT_CENTER);
	SelectObject(hdc, hOldFont);

	EndPaint(hwnd, &ps);
}

BOOL MainWnd::OnEraseBkgnd(HWND hwnd, HDC hdc)
{
	RECT rect;
	GetClientRect(hwnd, &rect);

	// 填充背景色
	FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	return TRUE;
}

LRESULT MainWnd::OnMyMsg(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	AdvCalc advCalc;
	LARGE_INTEGER freq, s0, s1, s2, s3;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&s0);
	float f1 = advCalc.calcBasic();
	QueryPerformanceCounter(&s1);
	float f2 = advCalc.calcSSE();
	QueryPerformanceCounter(&s2);
	float f3 = advCalc.calcAVX();
	QueryPerformanceCounter(&s3);

	float t1 = 1000.0f * (s1.QuadPart - s0.QuadPart) / freq.QuadPart;
	float t2 = 1000.0f * (s2.QuadPart - s1.QuadPart) / freq.QuadPart;
	float t3 = 1000.0f * (s3.QuadPart - s2.QuadPart) / freq.QuadPart;

	wchar_t* pbuf = m_wszBuf;
	int n, left = 512;
	n = swprintf_s(pbuf, left, L"%15s %15s %15s\n", L"NA", L"SSE 4x", L"AVX 8x");
	left -= n;
	n = swprintf_s(pbuf+n, left, L"%15.3f %15.3f %15.3f\n", f1, f2, f3);
	left -= n;
	swprintf_s(pbuf + n, left, L"%13.3fms %13.3fms %13.3fms", t1, t2, t3);

	InvalidateRect(hwnd, NULL, TRUE);

	return 0L;
}