#pragma once
using namespace std;

// 窗口基类
class XWnd
{
public:
	XWnd();
	virtual ~XWnd();

	operator HWND()const { return m_hwnd; }

	// 创建窗口
	virtual BOOL Create(wstring& wstrTitle, const RECT& rect, const XWnd& xwndParent, DWORD dwStyle);
	// 销毁窗口
	virtual void Destroy();
protected:
	// 窗口类型名称
	wstring m_wstrClsName;
	// 宋体
	HFONT m_hfontSong;
	HWND m_hwnd;

	// 注册窗口类型
	ATOM Register();

	// 窗口过程
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// 窗口消息处理
	virtual BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
	virtual void OnDestroy(HWND hwnd);
	virtual void OnPaint(HWND hwnd);
	virtual void OnSize(HWND hwnd, UINT state, int cx, int cy);
	virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
};

