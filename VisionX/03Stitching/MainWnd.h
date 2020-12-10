#pragma once
#include "XWnd.h"
#include "resource.h"
#include "Mate40.h"

using namespace std;

class CMainWnd final : public CXWnd
{
public:
    CMainWnd() {};
    ~CMainWnd() override {};

    ATOM Register(HINSTANCE hInstance) override;
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
private:
    static const wchar_t c_wszClsName[];
    // GDI+资源
    HDC m_hdcMem;
    HBITMAP m_hBmpMem;
    unique_ptr<Gdiplus::SolidBrush> m_uptrBrushBK;
    unique_ptr<Gdiplus::Pen> m_uptrPen;

    // 关键处理对象
    Mate40 m_mate40;

    // 后台工作线程
    thread m_thread;
    // 信号事件
    HANDLE m_evPuls;
    // 退出标志
    atomic_bool m_atomQuit;
    // 任务标志
    atomic_bool m_atomJob;

    // 保护区
    CRITICAL_SECTION m_cs;
    // 文件路径
    vector<wstring> m_vImagePaths;

    // 打开图像
    void pickImages();
    // 关闭图像
    void clearImages();
    // 工作线程
    void doWork();
    // wstring to string
    vector<string> convert(const vector<wstring>& vSrc);
private:
    // 调整图像绘制区域以保持原始比例
    static void resizeRectForImage(const unique_ptr<Gdiplus::Bitmap>& uptrBMP, Gdiplus::Rect& rc);
};

