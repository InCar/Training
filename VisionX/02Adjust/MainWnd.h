#pragma once
#include "XWnd.h"
#include "resource.h"
#include "CamCali.h"

class CMainWnd final: public CXWnd
{
public:
    CMainWnd();
    virtual ~CMainWnd();

    virtual ATOM Register(HINSTANCE hInstance);
    BOOL Create(HINSTANCE hInstance);
protected:
    LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) override;
    void OnDestroy(HWND hwnd) override;
    void OnPaint(HWND hwnd) override;
    BOOL OnEraseBkgnd(HWND hwnd, HDC hdc);
    void OnSize(HWND hwnd, UINT state, int cx, int cy);
    void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) override;

    void OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo);
    void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
    void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
    void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
private:
    static const wchar_t c_wszClsName[];
    // 尺寸系数
    const int m_nRightWidth = 200; // 右栏分割宽度
    const int m_nMargin = 4; // 边距
    float m_fLineHeight = -1.0f; // 行高,字体相关

    // GDI+资源
    HDC m_hdcMem;
    HBITMAP m_hBmpMem;
    std::unique_ptr<Gdiplus::Font> m_uptrFont;
    std::unique_ptr<Gdiplus::SolidBrush> m_uptrBrush;
    std::unique_ptr<Gdiplus::SolidBrush> m_uptrBrushBK;
    std::unique_ptr<Gdiplus::SolidBrush> m_uptrBrushFocus;
    std::unique_ptr<Gdiplus::Pen> m_uptrPen;

    bool m_bOpened = false; // 图像已经打开标志
    int m_nTracking = 0; // 当前滑块跟踪索引 0焦距 1~4系数K

    Gdiplus::Point m_pointA, m_pointB;
    Gdiplus::Rect m_rcBtn;
    bool m_bTracking = false;
    int m_nTrackingYDelta;

    // 系数
    const float m_fFocusMin = 50.0f, m_fFocusMax = 1500.0f;
    float m_fFocus = 800.0f;
    const float m_fdMin = -5.0f, m_fdMax = 5.0f;
    std::vector<float> m_fd4 = {0.0f, 0.0f, 0.0f, 0.0f};

    // 镜头修正处理对象
    std::unique_ptr<CamCali> m_uptrCC;

    // 打开图像
    void pickImage();
    // 计算图像绘制区域
    void calcRectForImage(Gdiplus::Rect& rc);
    // 计算焦距位置百分比
    float calcFocusPos();
    bool calcFocusPosBack(int y);
    // 计算系数K位置百分比
    float calcKPos(int n);
    bool calcKPosBack(int y);
};

