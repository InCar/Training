#pragma once
using namespace std;

class CXWnd
{
public:
    CXWnd();
    virtual ~CXWnd();

    operator HWND()const { return m_hwnd; }

    virtual BOOL Create(wstring &wstrTitle, const RECT &rect, const CXWnd &xwndParent, DWORD dwStyle);
    void Destroy();
protected:
    wstring m_wstrClsName;
    HWND m_hwnd;

    virtual ATOM Register();

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
    virtual void OnDestroy(HWND hwnd);
    virtual void OnPaint(HWND hwnd);
    virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
};

