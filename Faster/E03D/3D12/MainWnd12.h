#pragma once

using namespace Microsoft::WRL;

class MainWnd12 : public XWnd
{
public:
	MainWnd12();
	virtual ~MainWnd12();

	BOOL Create();
protected:
	ATOM Register();
	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	
	BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) override;
	void OnDestroy(HWND hwnd) override;
	virtual void OnPaint(HWND hwnd) override;
	virtual BOOL OnEraseBkgnd(HWND hwnd, HDC hdc);
private:
	ComPtr<ID3D12Device> m_ptrDev;
};

