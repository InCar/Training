#include "stdafx.h"
#include "MP3Player.h"
#include "resource.h"

CMP3Player::CMP3Player()
{
    m_reftDuration = 10000000*5; // in 100-nano-sec

    m_pwfex = NULL;
    m_pSourceReader = NULL;
    m_pMMDevice = NULL;
    m_pAudioClient = NULL;
    m_pRenderClient = NULL;
    m_pLastBuffer = NULL;
    m_pLastSrc = NULL;
    m_uLastBufferSize = 0;
}


CMP3Player::~CMP3Player()
{
}

LRESULT CMP3Player::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        HANDLE_MSG(hWnd, WM_TIMER, OnTimer);
        HANDLE_MSG(hWnd, WM_CTLCOLORSTATIC, OnCtlColor);
        break;
    default:
        return CXWnd::WndProc(hWnd, uMsg, wParam, lParam);
    }
}

BOOL CMP3Player::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    if (!CXWnd::OnCreate(hwnd, lpCreateStruct)) return FALSE;

    const int MARGIN = 4;
    int width = 74, height = 74;
    int top = (lpCreateStruct->cy - height) / 2;
    int left = lpCreateStruct->cx / 2 - width - MARGIN;

    m_hwndTitle = CreateWindow(L"Static", L"MP3", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, MARGIN, lpCreateStruct->cx, 32, hwnd, NULL, NULL, NULL);
    SendMessage(m_hwndTitle, WM_SETFONT, reinterpret_cast<WPARAM>(m_hfontSong), MAKELPARAM(TRUE, 0));

    m_hwndBtnPlay = CreateWindow(L"Button", L"PLAY", WS_CHILD | WS_VISIBLE, left, top, width, height, hwnd, (HMENU)ID_MP3_PLAY, NULL, NULL);
    SendMessage(m_hwndBtnPlay, WM_SETFONT, reinterpret_cast<WPARAM>(m_hfontSong), MAKELPARAM(TRUE, 0));

    left = lpCreateStruct->cx / 2 + MARGIN;
    m_hwndBtnStop = CreateWindow(L"Button", L"STOP", WS_CHILD | WS_VISIBLE, left, top, width, height, hwnd, (HMENU)ID_MP3_STOP, NULL, NULL);
    SendMessage(m_hwndBtnStop, WM_SETFONT, reinterpret_cast<WPARAM>(m_hfontSong), MAKELPARAM(TRUE, 0));

    // 创建根对象设备枚举器
    IMMDeviceEnumerator *pIMMDeviceEnumerator = NULL;
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&pIMMDeviceEnumerator));

    // 获取默认播放设备
    hr = pIMMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &m_pMMDevice);
    pIMMDeviceEnumerator->Release();
    return TRUE;
}

void CMP3Player::OnDestroy(HWND hwnd)
{
    // Force to stop
    SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_MP3_STOP, 0), 0);

    if (m_pMMDevice) m_pMMDevice->Release();
    m_pMMDevice = NULL;

    CXWnd::OnDestroy(hwnd);
}

void CMP3Player::OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    const int MARGIN = 4;
    RECT rc;
    GetClientRect(hwnd, &rc);

    int width = 74, height = 74;
    int top = (rc.bottom - height) / 2;
    int left = rc.right / 2 - width - MARGIN;

    SetWindowPos(m_hwndTitle, 0, 0, MARGIN, rc.right, 32, SWP_NOZORDER);
    SetWindowPos(m_hwndBtnPlay, 0, left, top, width, height, SWP_NOZORDER);
    SetWindowPos(m_hwndBtnStop, 0, rc.right / 2 + MARGIN, top, width, height, SWP_NOZORDER);
}

HBRUSH CMP3Player::OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{
    return GetStockBrush(NULL_BRUSH);
}

void CMP3Player::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case ID_MP3_PLAY:
    {
        if (m_pSourceReader) break;

        wstring wstrMP3;
        if (OpenMP3File(wstrMP3))
        {
            HRESULT hr = MFCreateSourceReaderFromURL(wstrMP3.c_str(), NULL, &m_pSourceReader);
            if (FAILED(hr)) {
                MessageBox(hwnd, L"打开文件失败", L"失败", MB_OK);
                break;
            }

            // 激活设备
            hr = m_pMMDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, reinterpret_cast<void**>(&m_pAudioClient));
            hr = m_pAudioClient->GetMixFormat(&m_pwfex); // 混合器格式

            // 解码器设定
            IMFMediaType *pMediaType = NULL;
            hr = MFCreateMediaType(&pMediaType);
            hr = MFInitMediaTypeFromWaveFormatEx(pMediaType, m_pwfex, sizeof(WAVEFORMATEXTENSIBLE));
            hr = m_pSourceReader->SetStreamSelection(MF_SOURCE_READER_ALL_STREAMS, FALSE);
            hr = m_pSourceReader->SetStreamSelection(MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE);
            hr = m_pSourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, pMediaType);
            pMediaType->Release();

            // Play
            hr = m_pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, m_reftDuration, 0, m_pwfex, NULL);
            hr = m_pAudioClient->GetService(__uuidof(IAudioRenderClient), reinterpret_cast<void**>(&m_pRenderClient));

            if (SUCCEEDED(hr)) {
                DWORD dwFlags = 0, dwLen = 0;
                IMFSample *pSample = NULL;
                IMFMediaBuffer *pMFBuffer = NULL;
                BYTE *pData = NULL, *pSrc = NULL;
                // read buffer
                hr = m_pSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, NULL, &dwFlags, NULL, &pSample);
                if (dwFlags & MF_SOURCE_READERF_ENDOFSTREAM) {
                    SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_MP3_STOP, 0), 0);
                    break;
                }

                hr = pSample->ConvertToContiguousBuffer(&pMFBuffer);
                hr = pMFBuffer->Lock(&pSrc, NULL, &dwLen);
                // write buffer
                UINT uFrameRead = dwLen / m_pwfex->nBlockAlign;
                hr = m_pRenderClient->GetBuffer(uFrameRead, &pData);
                memcpy(pData, pSrc, dwLen);
                hr = m_pRenderClient->ReleaseBuffer(uFrameRead, 0);
                pMFBuffer->Unlock();

                pMFBuffer->Release();
                pSample->Release();

                // 计算缓冲区可播放时间
                double dDuration = (double)uFrameRead / m_pwfex->nSamplesPerSec;
                hr = m_pAudioClient->Start();

                // 继续填充缓冲区
                SetTimer(hwnd, TIME_ID_PLAY, (UINT)(dDuration * 1000 * 0.8), NULL);
                Button_Enable(hwndCtl, FALSE);
            }
            Button_Enable(m_hwndBtnPlay, FALSE);
        }
        break;
    }
    case ID_MP3_STOP:
    {
        if (!m_pSourceReader) break;

        HRESULT hr = m_pAudioClient->Stop();
        KillTimer(hwnd, TIME_ID_PLAY);

        m_pRenderClient->Release();
        m_pRenderClient = NULL;

        m_pAudioClient->Release();
        m_pAudioClient = NULL;

        if (m_pLastBuffer) {
            m_pLastBuffer->Unlock();
            m_pLastBuffer->Release();
            m_uLastBufferSize = 0;
            m_pLastSrc = 0;
        }

        CoTaskMemFree(m_pwfex);
        m_pwfex = NULL;

        m_pSourceReader->Release();
        m_pSourceReader = NULL;

        Button_Enable(m_hwndBtnPlay, TRUE);
        break;
    }
    default:
        CXWnd::OnCommand(hwnd, id, hwndCtl, codeNotify);
    }
}

void CMP3Player::OnTimer(HWND hwnd, UINT id)
{
    switch (id)
    {
    case TIME_ID_PLAY:
    {
        DWORD dwFlags = 0, dwLen = 0;
        UINT uFrameRead = 0;
        HRESULT hr = S_OK;
        
        IMFMediaBuffer *pMFBuffer = NULL;
        BYTE *pData = NULL, *pSrc = NULL;
        // read buffer
        if (m_uLastBufferSize == 0) {
            hr = m_pSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, NULL, &dwFlags, NULL, &m_pSample);
            if (dwFlags & MF_SOURCE_READERF_ENDOFSTREAM) {
                // 放完了，待缓冲区放完就停止
                KillTimer(hwnd, TIME_ID_PLAY);
                UINT uFramesPadding = 0;
                hr = m_pAudioClient->GetCurrentPadding(&uFramesPadding); // 还没有播放的帧数
                SetTimer(hwnd, TIME_ID_STOP,static_cast<UINT>((double)uFramesPadding * 1000 / m_pwfex->nSamplesPerSec), NULL);
                break;
            }
            hr = m_pSample->ConvertToContiguousBuffer(&pMFBuffer);
            hr = pMFBuffer->Lock(&pSrc, NULL, &dwLen);
        }
        else {
            pMFBuffer = m_pLastBuffer;
            m_pLastBuffer = NULL;
            pSrc = m_pLastSrc;
            dwLen = m_uLastBufferSize;
        }

        // get buffer
        uFrameRead = dwLen / m_pwfex->nBlockAlign;
        hr = m_pRenderClient->GetBuffer(uFrameRead, &pData);

        if (FAILED(hr)) {
            m_pLastBuffer = pMFBuffer;
            m_pLastSrc = pSrc;
            m_uLastBufferSize = dwLen;
        }
        else {
            // write buffer
            memcpy(pData, pSrc, dwLen);
            hr = m_pRenderClient->ReleaseBuffer(uFrameRead, 0);
            pMFBuffer->Unlock();
            m_pLastSrc = NULL;
            m_uLastBufferSize = 0;
            m_pLastBuffer = NULL;

            pMFBuffer->Release();
            m_pSample->Release();
            m_pSample = NULL;

            // buffer trace
            wchar_t buf[256];
            static unsigned int c = 0;
            UINT uFramesPadding = 0;
            hr = m_pAudioClient->GetCurrentPadding(&uFramesPadding); // 还没有播放的帧数
            UINT msAdd = static_cast<UINT>((double)uFrameRead * 1000 / m_pwfex->nSamplesPerSec);
            UINT msTotal = static_cast<UINT>((double)uFramesPadding * 1000 / m_pwfex->nSamplesPerSec);
            StringCchPrintf(buf, 256, L"+%d[%dms]->%dms %s", uFrameRead, msAdd, msTotal, (c++) % 4 == 3 ? L"\n" : L"");
            OutputDebugString(buf);
        }

        
        break;
    }
    case TIME_ID_STOP:
    {
        KillTimer(hwnd, TIME_ID_STOP);
        SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_MP3_STOP, 0), 0);
        break;
    }
    default:
        break;
    }
}

BOOL CMP3Player::OpenMP3File(wstring &wstrMP3)
{
    BOOL bRet = FALSE;
    IFileDialog *pFileDialog = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
    if (SUCCEEDED(hr)) {
        COMDLG_FILTERSPEC filter[2];
        filter[0].pszName = L"mp3(*.mp3)";
        filter[0].pszSpec = L"*.mp3";
        filter[1].pszName = L"mkv(*.mkv)";
        filter[1].pszSpec = L"*.mkv";

        hr = pFileDialog->SetFileTypes(sizeof(filter)/sizeof(COMDLG_FILTERSPEC), filter);
        hr = pFileDialog->SetFileTypeIndex(1);
        hr = pFileDialog->Show(m_hwnd);
        if (SUCCEEDED(hr)) {
            IShellItem *psi = NULL;
            hr = pFileDialog->GetResult(&psi);
            if (SUCCEEDED(hr)) {
                PWSTR pwszFilePath = NULL;
                hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pwszFilePath);
                if (SUCCEEDED(hr)) {
                    wstrMP3.assign(pwszFilePath);
                    CoTaskMemFree(pwszFilePath);
                    bRet = TRUE;
                }
                psi->Release();
            }
        }
    }
    pFileDialog->Release();

    return bRet;
}