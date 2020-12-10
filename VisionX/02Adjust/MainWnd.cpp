#include "stdafx.h"
#include "MainWnd.h"
#include "App.h"

#define WM_MY_UPDATE WM_USER+0
const wchar_t CMainWnd::c_wszClsName[] = L"CMainWnd";

CMainWnd::CMainWnd()
{
}

CMainWnd::~CMainWnd()
{
}

ATOM CMainWnd::Register(HINSTANCE hInstance)
{
    WNDCLASS wcls;
    ZeroMemory(&wcls, sizeof(WNDCLASS));
    wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcls.lpfnWndProc = CXWnd::WindowProc;
    wcls.hInstance = hInstance;
    wcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcls.lpszClassName = c_wszClsName;
    wcls.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_APP));
    wcls.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_MAIN);

    return RegisterClass(&wcls);
}

BOOL CMainWnd::Create(HINSTANCE hInstance)
{
    int cxScreen = GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);
    int width = cxScreen > 800 ? 800 : cxScreen;
    int height = cyScreen > 600 ? 600 : cyScreen;

    WNDCLASS cls;
    if (!GetClassInfo(hInstance, c_wszClsName, &cls)) Register(hInstance);

    wstring wstrTitle(L"FishEye Camera");
    m_hwnd = CreateWindow(c_wszClsName, wstrTitle.c_str(), WS_OVERLAPPEDWINDOW|WS_VISIBLE,
        (cxScreen - width) / 2, (cyScreen - height) / 2, width, height,
        NULL, NULL, hInstance, this);
    if (!m_hwnd) {
        MessageBox(NULL, L"Create window failed", L"Error", MB_OK);
        return FALSE;
    }
    else {
        BOOL enable;
        DwmIsCompositionEnabled(&enable);
        if (enable) {
            DWMNCRENDERINGPOLICY policy = DWMNCRP_ENABLED;
            DwmSetWindowAttribute(m_hwnd, DWMWA_NCRENDERING_POLICY, &policy, sizeof(DWMNCRENDERINGPOLICY));
        }
    }

    return TRUE;
}

LRESULT CMainWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRet;
    switch (uMsg)
    {
        HANDLE_MSG(hWnd, WM_ERASEBKGND, OnEraseBkgnd);
        HANDLE_MSG(hWnd, WM_MOUSEMOVE, OnMouseMove);
        HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
        HANDLE_MSG(hWnd, WM_LBUTTONUP, OnLButtonUp);
        HANDLE_MSG(hWnd, WM_SIZE, OnSize);
        HANDLE_MSG(hWnd, WM_GETMINMAXINFO, OnGetMinMaxInfo);
    case WM_MY_UPDATE:
        {
            m_bOpened = true;
            // 更新图像
            InvalidateRect(hWnd, nullptr, FALSE);
            lRet = 0L;
        }
        break;
    default:
        lRet = CXWnd::WndProc(hWnd, uMsg, wParam, lParam);
        break;
    }

    return lRet;
}

BOOL CMainWnd::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    BOOL bRet = CXWnd::OnCreate(hwnd, lpCreateStruct);

    // 双缓冲
    HDC hdc = GetDC(hwnd);
    m_hdcMem = CreateCompatibleDC(hdc);
    ReleaseDC(hwnd, hdc);

    // 字体
    Gdiplus::FontFamily ff(L"宋体");
    m_uptrFont = std::unique_ptr<Gdiplus::Font>(new Gdiplus::Font(&ff, 12));
    // 画刷
    m_uptrBrush = std::unique_ptr<Gdiplus::SolidBrush>(new Gdiplus::SolidBrush(Gdiplus::Color::Black));
    m_uptrBrushBK = std::unique_ptr<Gdiplus::SolidBrush>(new Gdiplus::SolidBrush(Gdiplus::Color::WhiteSmoke));
    m_uptrBrushFocus = std::unique_ptr<Gdiplus::SolidBrush>(new Gdiplus::SolidBrush(Gdiplus::Color::Crimson));
    // 画笔
    m_uptrPen = std::unique_ptr<Gdiplus::Pen>(new Gdiplus::Pen(Gdiplus::Color::Black));

    return bRet;
}

void CMainWnd::OnDestroy(HWND hwnd)
{
    ReleaseDC(hwnd, m_hdcMem);
    DeleteBitmap(m_hBmpMem);
    CXWnd::OnDestroy(hwnd);

    PostQuitMessage(0);
}

void CMainWnd::OnPaint(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    Gdiplus::Graphics g(m_hdcMem);
    g.FillRectangle(m_uptrBrushBK.get(), 0, 0, rc.right, rc.bottom);

    // 获取行高
    if (m_fLineHeight < 0) m_fLineHeight = static_cast<float>(m_uptrFont->GetHeight(&g));

    // 分左右栏
    int posDiv = rc.right - m_nRightWidth;
    g.DrawLine(m_uptrPen.get(), posDiv, 0, posDiv, rc.bottom);

    if (m_bOpened) {
        // 左栏绘图
        Gdiplus::Rect rcImage(0, 0, rc.right-m_nRightWidth, rc.bottom);
        calcRectForImage(rcImage);
        Gdiplus::Bitmap* pBMP = m_uptrCC->LockBMP();
        g.DrawImage(pBMP, rcImage);
        m_uptrCC->UnlockBMP(pBMP);
    }

    // 右栏调节参数
    wchar_t wszBuf[64];
    Gdiplus::Brush* pBrush;

    Gdiplus::PointF point(static_cast<float>(rc.right - m_nRightWidth + m_nMargin), 1.0f * m_nMargin);
    float fDeltaH = m_fLineHeight + m_nMargin;

    swprintf_s(wszBuf, L"%3s %4.2f", L"焦距", m_fFocus);
    pBrush = m_nTracking == 0 ? m_uptrBrushFocus.get() : m_uptrBrush.get();
    g.DrawString(wszBuf, -1, m_uptrFont.get(), point, pBrush);

    for (int i = 0; i < 4; i++) {
        swprintf_s(wszBuf, L"%4s%d %4.2f", L"d", i + 1, m_fd4[i]);
        pBrush = m_nTracking == i + 1 ? m_uptrBrushFocus.get() : m_uptrBrush.get();
        point.Y += fDeltaH;
        g.DrawString(wszBuf, -1, m_uptrFont.get(), point, pBrush);
    }

    // 调节滑块
    const int side = 16; // 滑块边长
    m_pointA = Gdiplus::Point(rc.right - m_nRightWidth / 2, static_cast<int>(point.Y + fDeltaH));
    m_pointB = Gdiplus::Point(rc.right - m_nRightWidth / 2, static_cast<int>(rc.bottom - m_nMargin));
    g.DrawLine(m_uptrPen.get(), m_pointA, m_pointB);

    float fTracking;
    if (m_nTracking == 0) { // 焦距
        fTracking = calcFocusPos();
    }
    else { // 系数K
        fTracking = calcKPos(m_nTracking - 1);
    }
    float posY = fTracking * (m_pointB.Y - m_pointA.Y - side) + m_pointA.Y;

    m_rcBtn = Gdiplus::Rect(m_pointA.X - side / 2, static_cast<int>(posY), side, side);
    pBrush = m_bTracking ? m_uptrBrushFocus.get() : m_uptrBrush.get();
    g.FillRectangle(pBrush, m_rcBtn);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    BitBlt(hdc, 0, 0, rc.right, rc.bottom, m_hdcMem, 0, 0, SRCCOPY);
    EndPaint(hwnd, &ps);
}

BOOL CMainWnd::OnEraseBkgnd(HWND hwnd, HDC hdc)
{
    return TRUE;
}

void CMainWnd::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    HDC hdc = GetDC(hwnd);
    m_hBmpMem = CreateCompatibleBitmap(hdc, cx, cy);
    HBITMAP hBmpOld = SelectBitmap(m_hdcMem, m_hBmpMem);
    DeleteBitmap(hBmpOld);
}

void CMainWnd::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case ID_FILE_OPEN:
        pickImage();
        break;
    case ID_FILE_EXIT:
        SendMessage(hwnd, WM_CLOSE, 0, 0);
        break;
    default:
        CXWnd::OnCommand(hwnd, id, hwndCtl, codeNotify);
        break;
    }
}

void CMainWnd::OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo)
{
    lpMinMaxInfo->ptMinTrackSize.x = 400;
    lpMinMaxInfo->ptMinTrackSize.y = 300;
}

void CMainWnd::OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    if (x >= m_rcBtn.GetLeft() && x <= m_rcBtn.GetRight()
        && y >= m_rcBtn.GetTop() && y <= m_rcBtn.GetBottom()) {
        // hit
        SetCapture(hwnd);
        m_bTracking = true;
        m_nTrackingYDelta = m_rcBtn.Y - y;

        InvalidateRect(hwnd, nullptr, TRUE);
    }
}

void CMainWnd::OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
    RECT rc;
    GetClientRect(hwnd, &rc);
    if (m_fLineHeight > 0 && x > rc.right - m_nRightWidth) {
        // 点中了右侧区域
        int nHit = y / (static_cast<int>(m_fLineHeight) + m_nMargin);
        if (nHit < 5 && m_nTracking != nHit) {
            // 切换
            m_nTracking = nHit;
            InvalidateRect(hwnd, nullptr, TRUE);
        }
    }

    if (m_bTracking) {
        ReleaseCapture();
        m_bTracking = false;
        InvalidateRect(hwnd, nullptr, TRUE);
    }
}

void CMainWnd::OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
    if (m_bTracking) {
        int yNew = y + m_nTrackingYDelta;

        bool bChanged = false;
        if (m_nTracking == 0)
            bChanged = calcFocusPosBack(yNew);
        else
            bChanged = calcKPosBack(yNew);

        if (bChanged)
            InvalidateRect(hwnd, nullptr, FALSE);
    }
}

void CMainWnd::pickImage()
{
    wchar_t wcsFile[MAX_PATH] = L"\0";

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = m_hwnd;
    ofn.lpstrFilter = L"JPEG(*.jpg;*.jpeg)\0*.jpg;*.jpeg\0\0";
    ofn.lpstrFile = wcsFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_ENABLESIZING | OFN_EXPLORER;

    if (GetOpenFileName(&ofn)) {
        // convert to multi-bytes
        char szbuf[MAX_PATH + 1];
        memset(szbuf, 0, sizeof(szbuf));
        WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, ofn.lpstrFile, static_cast<int>(wcslen(ofn.lpstrFile)), szbuf, MAX_PATH, NULL, FALSE);

        // reset
        m_bOpened = false;
        if (m_uptrCC != nullptr) m_uptrCC.reset();

        // create the CamCali(Camera Calibration)
        m_uptrCC = std::unique_ptr<CamCali>(new CamCali());
        m_uptrCC->OpenCameraImage(szbuf, [=]() {
            PostMessage(m_hwnd, WM_MY_UPDATE, 0, 0);
        });
        m_uptrCC->UpdateCoefficients(m_fFocus, m_fd4);
    }
}

void CMainWnd::calcRectForImage(Gdiplus::Rect& rc)
{
    // 计算图片绘制区域 保持纵横比例不变
    float fRatioRC = 1.0f * rc.Width / rc.Height;

    Gdiplus::Bitmap *pBMP = m_uptrCC->LockBMP();
    float fRatioIMG = 1.0f * pBMP->GetWidth() / pBMP->GetHeight();

    if (fRatioRC >= fRatioIMG) {
        // 绘制区域比图像宽,图像垂直方向占满
        float fWidth = 1.0f * pBMP->GetWidth() * rc.Height / pBMP->GetHeight();
        rc.X = static_cast<int>((rc.Width - fWidth) / 2.0f);
        rc.Width = static_cast<int>(fWidth);
    }
    else {
        // 绘制区域比图像窄,图像水平方向占满
        float fHeight = 1.0f * pBMP->GetHeight() * rc.Width / pBMP->GetWidth();
        rc.Y = static_cast<int>((rc.Height - fHeight) / 2.0f);
        rc.Height = static_cast<int>(fHeight);
    }
    m_uptrCC->UnlockBMP(pBMP);
}

float CMainWnd::calcFocusPos()
{
    // 50~1500
    return (m_fFocus - m_fFocusMin) / (m_fFocusMax - m_fFocusMin);
}

bool CMainWnd::calcFocusPosBack(int y)
{
    // 50~1500
    if (y >= m_pointA.Y && y <= m_pointB.Y) {
        m_fFocus = (m_fFocusMax - m_fFocusMin) * (y - m_pointA.Y) / (m_pointB.Y - m_pointA.Y) + m_fFocusMin;
        if(m_bOpened)
            m_uptrCC->UpdateCoefficients(m_fFocus, m_fd4);
        return true;
    }
    return false;
}

float CMainWnd::calcKPos(int n)
{
    // -2~+2
    return (m_fd4[n] - m_fdMin) / (m_fdMax - m_fdMin);
}

bool CMainWnd::calcKPosBack(int y)
{
    // -2~+2
    if (y >= m_pointA.Y && y <= m_pointB.Y) {
        m_fd4[m_nTracking - 1] = (m_fdMax - m_fdMin) * (y - m_pointA.Y) / (m_pointB.Y - m_pointA.Y) + m_fdMin;
        if (m_bOpened)
            m_uptrCC->UpdateCoefficients(m_fFocus, m_fd4);
        return true;
    }
    return false;
}
