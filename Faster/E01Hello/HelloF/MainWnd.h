#pragma once
#include "XWnd.h"

class MainWnd :	public XWnd
{
public:
	MainWnd();
	~MainWnd() override;

	BOOL Create();
protected:
	ATOM Register();

	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	void OnPaint(HWND hwnd) override;
	virtual BOOL OnEraseBkgnd(HWND hwnd, HDC hdc);
};

