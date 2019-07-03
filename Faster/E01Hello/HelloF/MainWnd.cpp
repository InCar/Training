#include "stdafx.h"
#include "MainWnd.h"
#include "App.h"
#include "resource.h"

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

	return TRUE;
}

LRESULT MainWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
		HANDLE_MSG(hWnd, WM_ERASEBKGND, OnEraseBkgnd);
		break;
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
	HGDIOBJ hOldFont = SelectObject(hdc, m_hfontSong);
	DrawText(hdc, L"你好,C++!", -1, &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
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