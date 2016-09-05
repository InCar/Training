#include "stdafx.h"
#include "resource.h"

#define WM_USER_IMG WM_USER + 6
HDC g_hdcMem = NULL;
SIZE g_szBmp;
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void LoadBMP(wchar_t*, HWND);
void LoadJPG(wchar_t*, HWND);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
    wchar_t wszTitle[] = L"Bitmap";

    WNDCLASS wcls;
    ZeroMemory(&wcls, sizeof(WNDCLASS));
    wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcls.lpfnWndProc = WindowProc;
    wcls.hInstance = hInstance;
    wcls.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wcls.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_APP));
    wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcls.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_MAIN);
    wcls.lpszClassName = wszTitle;

    if (!RegisterClass(&wcls)) {
        MessageBox(NULL, L"Register class failed", L"Error", MB_OK);
        return GetLastError();
    }

    int cxScreen = GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);
    int width = cxScreen > 800 ? 800 : cxScreen;
    int height = cyScreen > 600 ? 600 : cyScreen;

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        MessageBox(NULL, L"CoInitialize failed", L"Error", MB_OK);
        return GetLastError();
    }

    HWND hWndMain = CreateWindow(wszTitle, L"位图", WS_OVERLAPPEDWINDOW, (cxScreen - width) / 2, (cyScreen - height) / 2, width, height, NULL, NULL, hInstance, NULL);
    if (!hWndMain) {
        MessageBox(NULL, L"Create window failed", L"Error", MB_OK);
        return GetLastError();
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CoUninitialize();

    return (int)msg.wParam;
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    ShowWindow(hwnd, SW_SHOW);
    return TRUE;
}

void OnDestroy(HWND hwnd)
{
    if (g_hdcMem != NULL) {
        HBITMAP hBmpOld = SelectBitmap(g_hdcMem, NULL);
        if (hBmpOld != NULL) DeleteBitmap(hBmpOld);
        DeleteDC(g_hdcMem);
    }
    PostQuitMessage(0);
}

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    RECT rc;
    HDC hdc = BeginPaint(hwnd, &ps);
    GetClientRect(hwnd, &rc);
    if (g_hdcMem != NULL) {
        SetStretchBltMode(hdc, HALFTONE);
        SetBrushOrgEx(hdc, 0, 0, NULL);
        StretchBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, g_hdcMem, 0, 0, g_szBmp.cx, g_szBmp.cy, SRCCOPY);
    }
    EndPaint(hwnd, &ps);
}

BOOL OnEraseBkgnd(HWND hwnd, HDC hdc)
{
    if (g_hdcMem == NULL) {
        RECT rc;
        GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
    }
    return TRUE;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id) {
    case ID_FILE_OPEN:
    {
        IFileDialog *pFileDialog = NULL;
        HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
        if (SUCCEEDED(hr)) {
            COMDLG_FILTERSPEC filter[3];
            filter[0].pszName = L"位图文件(*.bmp)";
            filter[0].pszSpec = L"*.bmp";
            filter[1].pszName = L"JPEG(*.jpg;*.jpeg)";
            filter[1].pszSpec = L"*.jpg;*.jpeg";
            filter[2].pszName = L"所有支持的文件(*.jpg;*.jpeg;*.bmp;)";
            filter[2].pszSpec = L"*.jpg;*.jpeg;*.bmp;*.png";

            hr = pFileDialog->SetFileTypes(3, filter);
            hr = pFileDialog->SetFileTypeIndex(3);
            hr = pFileDialog->Show(hwnd);
            if (SUCCEEDED(hr)) {
                IShellItem *psi = NULL;
                hr = pFileDialog->GetResult(&psi);
                if (SUCCEEDED(hr)) {
                    PWSTR pwszFilePath = NULL;
                    hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pwszFilePath);
                    if (SUCCEEDED(hr)) {
                        wchar_t * pext = wcsrchr(pwszFilePath, L'.');
                        if(pext != NULL && _wcsicmp(pext, L".bmp") == 0)
                            SendMessage(hwnd, WM_USER_IMG, 0, (LPARAM)pwszFilePath);
                        else
                            SendMessage(hwnd, WM_USER_IMG, 1, (LPARAM)pwszFilePath);
                        CoTaskMemFree(pwszFilePath);
                    }
                    psi->Release();
                }
            }
        }
        pFileDialog->Release();
        
        break;
    }
    case ID_FILE_EXIT:
        PostQuitMessage(0);
        break;
    default:
        FORWARD_WM_COMMAND(hwnd, id, hwndCtl, codeNotify, DefWindowProc);
    }
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hWnd, WM_ERASEBKGND, OnEraseBkgnd);
    case WM_USER_IMG:
    {
        wchar_t *pwszFile = (wchar_t*)lParam;
        if (wParam == 0)
            LoadBMP(pwszFile, hWnd);
        else
            LoadJPG(pwszFile, hWnd);
        break;
    }
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0L;
}

void MakeMemDC(HBITMAP hBmp, HWND hwnd)
{
    if (hBmp == NULL) return;

    BITMAP bmp;
    GetObject(hBmp, sizeof(BITMAP), &bmp);
    g_szBmp.cx = bmp.bmWidth;
    g_szBmp.cy = bmp.bmHeight;

    if (g_hdcMem == NULL) {
        HDC hdc = GetDC(hwnd);
        g_hdcMem = CreateCompatibleDC(hdc);
        ReleaseDC(hwnd, hdc);
    }

    HBITMAP hBmpOld = SelectBitmap(g_hdcMem, hBmp);
    if (hBmpOld != NULL) DeleteObject(hBmpOld);
}

void LoadBMP(wchar_t *pwszBmp, HWND hwnd)
{
    // read the bmp content
    HBITMAP hBmp = (HBITMAP)LoadImage(NULL, pwszBmp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    MakeMemDC(hBmp, hwnd);

    InvalidateRect(hwnd, NULL, TRUE);
}

void LoadJPG(wchar_t *pwszJpg, HWND hwnd)
{
    IWICImagingFactory *pFactory = NULL;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));
    if (SUCCEEDED(hr)) {
        IWICBitmapDecoder *pDecoder = NULL;
        hr = pFactory->CreateDecoderFromFilename(pwszJpg, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
        if (SUCCEEDED(hr)) {
            IWICBitmapFrameDecode *pIDecoderFrame = NULL;
            hr = pDecoder->GetFrame(0, &pIDecoderFrame);
            if (SUCCEEDED(hr)) {
                IWICFormatConverter *pFC = NULL;
                hr = pFactory->CreateFormatConverter(&pFC);
                hr = pFC->Initialize(pIDecoderFrame, GUID_WICPixelFormat24bppBGR, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);

                BITMAPINFO bmpInfo;
                ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
                bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                hr = pIDecoderFrame->GetSize((UINT*)&bmpInfo.bmiHeader.biWidth, (UINT*)&bmpInfo.bmiHeader.biHeight);
                bmpInfo.bmiHeader.biPlanes = 1;
                bmpInfo.bmiHeader.biBitCount = 24;
                bmpInfo.bmiHeader.biCompression = BI_RGB;

                BYTE *pBuf = NULL;
                HDC hdc = GetDC(hwnd);
                bmpInfo.bmiHeader.biHeight *= -1; // BMP 方向调整
                HBITMAP hBmp = CreateDIBSection(hdc, &bmpInfo, DIB_RGB_COLORS, (void**)&pBuf, NULL, 0);
                bmpInfo.bmiHeader.biHeight *= -1;

                // 内部格式 按4字节边界对齐
                /*unsigned int cbStride = 4 * (bmpInfo.bmiHeader.biWidth * bmpInfo.bmiHeader.biBitCount + 31) / 32;
                unsigned int total = cbStride * bmpInfo.bmiHeader.biHeight;
                hr = pIDecoderFrame->CopyPixels(NULL, cbStride, total, pBuf);*/

                // 普通方式
                unsigned int cbStride = 3 * bmpInfo.bmiHeader.biWidth;
                unsigned int total = cbStride*bmpInfo.bmiHeader.biHeight;
                hr = pIDecoderFrame->CopyPixels(NULL, cbStride, total, pBuf);


                DirectX::XMVECTORF32 colorLuminance = { 1.0f, 1.0f, 1.0f, 1.0f };

                if (SUCCEEDED(hr)) {
                    hdc = GetDC(hwnd);
                    for (int i = 0; i < bmpInfo.bmiHeader.biHeight; i++) {
                        for (int j = 0; j < bmpInfo.bmiHeader.biWidth; j++) {
                            BYTE *pColor = pBuf + cbStride*i + j * 3;
                            BYTE blue = *(pColor + 0), green = *(pColor + 1), red = *(pColor + 2);

                            DirectX::XMVECTORF32 colorRGB = { red/255.0f, green/255.0f, blue/255.0f, 1.0f};
                            DirectX::XMVECTOR colorHSL = DirectX::XMColorRGBToHSL(colorRGB);
                            colorHSL = DirectX::XMColorModulate(colorHSL, colorLuminance);
                            DirectX::XMVECTOR colorRGB2 = DirectX::XMColorHSLToRGB(colorHSL);
                            DirectX::XMFLOAT4A colorRGB3;
                            DirectX::XMStoreFloat4A(&colorRGB3, colorRGB2);
                            pColor[0] = (BYTE)(colorRGB3.z * 255); // blue
                            pColor[1] = (BYTE)(colorRGB3.y * 255); // green
                            pColor[2] = (BYTE)(colorRGB3.x * 255); // red

                            SetPixel(hdc, j, i, RGB(pColor[2], pColor[1], pColor[0]));
                        }
                    }
                }

                if (SUCCEEDED(hr)) {
                    MakeMemDC(hBmp, hwnd);
                    InvalidateRect(hwnd, NULL, TRUE);
                }

                pIDecoderFrame->Release();
            }
            pDecoder->Release();
        }

        pFactory->Release();
    }
}