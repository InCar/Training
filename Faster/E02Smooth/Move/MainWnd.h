#pragma once
#include "Car.h"

class MainWnd : public XWnd
{
public:
	MainWnd();
	virtual ~MainWnd();

	BOOL Create();
protected:
	ATOM Register();
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	
	BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) override;
	void OnDestroy(HWND hwnd) override;
	virtual void OnPaint(HWND hwnd) override;
	virtual void OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
	virtual LRESULT OnCarMoving(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	Car m_car;
	HPEN m_hDotPen;
	HBRUSH m_hbrCar;
};

