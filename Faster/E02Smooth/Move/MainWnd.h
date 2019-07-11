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
	virtual BOOL OnEraseBkgnd(HWND hwnd, HDC hdc);
	virtual void OnKeyUpDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
	virtual LRESULT OnCarMoving(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	const int c_iWorldWidth  = 1000 * 100; // 单位:毫米
	const int c_iWorldLength = 1000 * 100; // 单位:毫米
	const float c_fMaxFPS = 100.0f;
	Car m_car;
	HPEN m_hDotPen;
	HBRUSH m_hbrCar;
	HDC m_hdcMem;
	HBITMAP m_hBmp;
	LARGE_INTEGER m_i64Freq;

	// 渲染
	void Render(HDC& hdc, RECT& rc);
	// 设定坐标系
	void ChangeMapMode(const HDC& hdc, int pixels, RECT& rc);
	// 转换到逻辑坐标系
	inline void ToWorld(const HDC& hdc, RECT& rc);
};

