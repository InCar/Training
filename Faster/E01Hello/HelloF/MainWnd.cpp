#include "stdafx.h"
#include "resource.h"
#include "MainWnd.h"
#include "App.h"
#include "AdvCalc.h"

MainWnd::MainWnd()
{
	ZeroMemory(m_wszBuf, sizeof(m_wszBuf));
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

	const int N = 5;
	float f[N] = {0.0f}, t[N];
	LARGE_INTEGER freq, s[N+1];
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&s[0]);
	f[0] = advCalc.calcBasic();
	QueryPerformanceCounter(&s[1]);
	if (advCalc.IsSupport(SIMD::SSE)) f[1] = advCalc.calcSSE();
	QueryPerformanceCounter(&s[2]);
	if(advCalc.IsSupport(SIMD::AVX)) f[2] = advCalc.calcAVX();
	QueryPerformanceCounter(&s[3]);
	if (advCalc.IsSupport(SIMD::FMA)) f[3] = advCalc.calcFMA();
	QueryPerformanceCounter(&s[4]);
	if (advCalc.IsSupport(SIMD::AVX512)) f[4] = advCalc.calcAVX512();
	QueryPerformanceCounter(&s[5]);

	for (int i = 0;i < N;i++) {
		t[i] = 1000.0f * (s[i+1].QuadPart - s[i].QuadPart) / freq.QuadPart;
	} 

	wchar_t* pbuf = m_wszBuf;
	int n, left = 512;
	n = swprintf_s(pbuf, left, L"%15s %15s %15s %15s %15s\n", L"NA", L"SSE 4x", L"AVX 8x", L"FMA 8x+", L"AVX512 16x+");
	left -= n;
	pbuf += n;
	n = swprintf_s(pbuf, left, L"%15.3f %15.3f %15.3f %15.3f %15.3f\n", f[0], f[1], f[2], f[3], f[4]);
	left -= n;
	pbuf += n;
	swprintf_s(pbuf, left, L"%13.3fms %13.3fms %13.3fms %13.3fms %13.3fms", t[0], t[1], t[2], t[3], t[4]);

	InvalidateRect(hwnd, NULL, TRUE);

	return 0L;
}