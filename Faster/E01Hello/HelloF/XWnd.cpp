#include "stdafx.h"
#include "XWnd.h"
#include "App.h"

XWnd::XWnd()
	:m_wstrClsName(L"XWnd")
{
}


XWnd::~XWnd()
{
}

ATOM XWnd::Register()
{
	// 注册窗口类型
	HINSTANCE hInstance = App::GetApp()->GetInstance();

	WNDCLASS wcls;
	ZeroMemory(&wcls, sizeof(WNDCLASS));
	wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcls.lpfnWndProc = XWnd::WindowProc;
	wcls.hInstance = hInstance;
	wcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcls.lpszClassName = m_wstrClsName.c_str();

	return RegisterClass(&wcls);
}

BOOL XWnd::Create(wstring& wstrTitle, const RECT& rect, const XWnd& xwndParent, DWORD dwStyle)
{
	// 创建窗口
	HINSTANCE hInstance = App::GetApp()->GetInstance();

	WNDCLASS cls;
	if (!GetClassInfo(hInstance, m_wstrClsName.c_str(), &cls)) Register();

	m_hwnd = CreateWindow(m_wstrClsName.c_str(), wstrTitle.c_str(),
		dwStyle | WS_CHILD | WS_VISIBLE | WS_BORDER,
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
		xwndParent, NULL, hInstance, this);
	if (!m_hwnd) {
		MessageBox(xwndParent, L"Create window failed", L"Error", MB_OK);
		return FALSE;
	}
	else {
		return TRUE;
	}
}

void XWnd::Destroy()
{
	// 销毁窗口
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;
}

LRESULT XWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 消息处理
	switch (uMsg) {
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT XWnd::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 挂接窗口过程
	XWnd* pXWnd = NULL;
	if (uMsg == WM_CREATE) {
		LPVOID lpCreateParams = reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams;
		pXWnd = reinterpret_cast<XWnd*>(lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pXWnd));
	}
	else {
		pXWnd = reinterpret_cast<XWnd*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if (!pXWnd) return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return pXWnd->WndProc(hWnd, uMsg, wParam, lParam);
}

BOOL XWnd::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	// 创建字体"宋体"
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = 12;
	lf.lfWeight = FW_MEDIUM;
	wcscpy_s(lf.lfFaceName, LF_FACESIZE, L"宋体");
	m_hfontSong = CreateFontIndirect(&lf);

	return TRUE;
}

void XWnd::OnDestroy(HWND hwnd)
{
	// 销毁字体
	DeleteFont(m_hfontSong);
	PostQuitMessage(0);
}

void XWnd::OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	EndPaint(hwnd, &ps);
}

void XWnd::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
}

void XWnd::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, DefWindowProc);
}