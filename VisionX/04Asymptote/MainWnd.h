#pragma once
#include "XWnd.h"
#include "resource.h"
#include "RX6000.h"
#include "CVBitmap.h"

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
    void OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);

    void OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo);
private:
    // 分割线位置
    float m_fSeparator = 0.7f;

    // GDI+资源
    HDC m_hdcMem;
    HBITMAP m_hBmpMem;
    std::unique_ptr<Gdiplus::SolidBrush> m_uptrBrushBK;
    std::unique_ptr<Gdiplus::Pen> m_uptrPenGray;
    std::unique_ptr<Gdiplus::Pen> m_uptrPenCrimson;
    std::unique_ptr<Gdiplus::Pen> m_uptrPenOrange;
    std::unique_ptr<Gdiplus::Font> m_uptrFontSong;
    std::unique_ptr<Gdiplus::SolidBrush> m_uptrBrushText;

    // 计算默认窗口位置
    RECT calcDefaultWindowRect();
    // 打开图像
    void pickImages();
    // 关闭图像
    void clearImages();
    // 绘制鱼眼theta曲线
    void drawCurve(Gdiplus::Graphics& g, const Gdiplus::Rect& rc);
private:
    // 工作信号
    HANDLE m_evWakeUp;
    // 退出标志
    std::atomic_bool m_atomExit;
    // 工作线程
    std::thread m_thread;

    // 保护
    CRITICAL_SECTION m_csRX6K;
    // 待处理图片路径
    std::vector<std::wstring> m_vPaths;
    // 鱼眼系数
    std::vector<float> m_vk = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    // 135等效焦距(mm)
    float m_fFocus135 = 15.0f;
    // 水平偏角
    std::vector<float> m_vYaw = { 0.0f, 0.0f, 0.0f, 0.0f };
    // 俯仰角
    std::vector<float> m_vPitch = { -0.47f, -0.3f, 0.0f, 0.0f };
    // 系数调节步长
    float m_fStep = 0.1f;
    // 输出的图像
    std::unique_ptr<CVBitmap> m_uptrOutputBitmap;
    // 输入的图像或中间结果
    std::vector<std::unique_ptr<CVBitmap>> m_vuptrBitmaps;

    // 关键处理对象
    RX6000 m_rx6k;

    // 后台任务
    void doWork();
    std::vector<std::wstring> doWorkForInput(float& f135, std::vector<float>& vk, std::vector<float>& vPitch, std::vector<float>& vYaw);
    void doWorkForOutput(bool bUpdateMiddle, bool bUpdateFinal);
private:
    static const wchar_t c_wszClsName[];
    // 调整图像绘制区域以保持原始比例
    static void resizeRectForImage(Gdiplus::Bitmap& bmp, Gdiplus::Rect& rc);
    // w2a
    static std::vector<std::string> convert(const std::vector<std::wstring>& vSrcW);
};

