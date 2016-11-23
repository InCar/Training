#include "stdafx.h"
#include "SineWnd.h"
#include "resource.h"

CSineWnd::CSineWnd()
{
    m_reftDuration = 10000000; // in 100-nano-sec
    m_wfex.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
    m_wfex.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE);
    m_wfex.Format.nChannels = 2;
    m_wfex.Format.nSamplesPerSec = 48000;
    m_wfex.Format.wBitsPerSample = 32;
    m_wfex.Format.nBlockAlign = m_wfex.Format.wBitsPerSample*m_wfex.Format.nChannels / 8;
    m_wfex.Format.nAvgBytesPerSec = m_wfex.Format.nBlockAlign * m_wfex.Format.nSamplesPerSec;
    m_wfex.Samples.wSamplesPerBlock = m_wfex.Format.wBitsPerSample;
    m_wfex.dwChannelMask = SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT;
    m_wfex.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

    m_pMMDevice = NULL;
    m_pAudioClient = NULL;
    m_pRenderClient = NULL;

    m_uFrameNow = 0;
}


CSineWnd::~CSineWnd()
{
}

LRESULT CSineWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        HANDLE_MSG(hWnd, WM_TIMER, OnTimer);
        HANDLE_MSG(hWnd, WM_CTLCOLORSTATIC, OnCtlColor);
        break;
    default:
        return CXWnd::WndProc(hWnd, uMsg, wParam, lParam);
    }
}

BOOL CSineWnd::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    if (!CXWnd::OnCreate(hwnd, lpCreateStruct)) return FALSE;

    const int MARGIN = 4;
    int width = 74, height = 74;
    int top = (lpCreateStruct->cy - height) / 2;
    int left = lpCreateStruct->cx / 2 - width - MARGIN;
    
    m_hwndTitle = CreateWindow(L"Static", L"正弦波", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, MARGIN, lpCreateStruct->cx, 32, hwnd, NULL, NULL, NULL);
    SendMessage(m_hwndTitle, WM_SETFONT, reinterpret_cast<WPARAM>(m_hfontSong), MAKELPARAM(TRUE, 0));

    m_hwndBtnPlay = CreateWindow(L"Button", L"PLAY", WS_CHILD | WS_VISIBLE, left, top, width, height, hwnd, (HMENU)ID_AUDIO_PLAY, NULL, NULL);
    SendMessage(m_hwndBtnPlay, WM_SETFONT, reinterpret_cast<WPARAM>(m_hfontSong), MAKELPARAM(TRUE, 0));

    left = lpCreateStruct->cx / 2 + MARGIN;
    m_hwndBtnStop = CreateWindow(L"Button", L"STOP", WS_CHILD | WS_VISIBLE, left, top, width, height, hwnd, (HMENU)ID_AUDIO_STOP, NULL, NULL);
    SendMessage(m_hwndBtnStop, WM_SETFONT, reinterpret_cast<WPARAM>(m_hfontSong), MAKELPARAM(TRUE, 0));

    // 创建根对象设备枚举器
    IMMDeviceEnumerator *pIMMDeviceEnumerator = NULL;
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&pIMMDeviceEnumerator));

    // 获取默认播放设备
    hr = pIMMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &m_pMMDevice);

    pIMMDeviceEnumerator->Release();
    return TRUE;
}

void CSineWnd::OnDestroy(HWND hwnd)
{
    // Force to stop
    SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_AUDIO_STOP, 0), 0);

    m_pMMDevice->Release();
    m_pMMDevice = NULL;

    CXWnd::OnDestroy(hwnd);
}

void CSineWnd::OnSize(HWND hwnd, UINT state, int cx, int cy)
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

HBRUSH CSineWnd::OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{
    return GetStockBrush(NULL_BRUSH);
}

void CSineWnd::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case ID_AUDIO_PLAY:
    {
        if (m_pAudioClient) break; // 已经开始播放了

        UINT uFrameCount = 0;
        BYTE *pData = NULL;

        // 激活设备
        HRESULT hr = m_pMMDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, reinterpret_cast<void**>(&m_pAudioClient));
        WAVEFORMATEX *pwfex = NULL;
        hr = m_pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, reinterpret_cast<WAVEFORMATEX*>(&m_wfex), &pwfex);
        if (hr != S_OK) {
            MessageBox(hwnd, L"不支持的音频格式-48KHz双声道16bits", L"Error", MB_OK);
            m_pAudioClient->Release();
            m_pAudioClient = NULL;
            return;
        }

        hr = m_pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, m_reftDuration, 0, reinterpret_cast<WAVEFORMATEX*>(&m_wfex), NULL);
        hr = m_pAudioClient->GetService(__uuidof(IAudioRenderClient), reinterpret_cast<void**>(&m_pRenderClient));
        hr = m_pAudioClient->GetBufferSize(&uFrameCount);
        hr = m_pRenderClient->GetBuffer(uFrameCount, &pData);
        if (SUCCEEDED(hr)) {
            // fill buffer
            float *pFloatData = reinterpret_cast<float*>(pData);
            for (unsigned int i = 0; i < uFrameCount; i++) {
                *pFloatData = sinf(400 * 2 * 3.1415927f*i / m_wfex.Format.nSamplesPerSec);
                *(pFloatData + 1) = *pFloatData;
                pFloatData += 2;
            }

            m_uFrameNow = uFrameCount;
            hr = m_pRenderClient->ReleaseBuffer(uFrameCount, 0);

            // 计算缓冲区可播放时间
            double dDuration = (double)uFrameCount / m_wfex.Format.nSamplesPerSec;
            hr = m_pAudioClient->Start();

            // 大约播放一半时，继续填充缓冲区
            SetTimer(hwnd, TIME_ID_PLAY, (UINT)(dDuration * 1000 / 2), NULL);
            Button_Enable(hwndCtl, FALSE);
        }
        break;
    }
    case ID_AUDIO_STOP:
    {
        if (!m_pAudioClient) break;

        HRESULT hr = m_pAudioClient->Stop();
        KillTimer(hwnd, TIME_ID_PLAY);

        m_pRenderClient->Release();
        m_pRenderClient = NULL;

        m_pAudioClient->Release();
        m_pAudioClient = NULL;
        
        Button_Enable(m_hwndBtnPlay, TRUE);
        break;
    }
    default:
        CXWnd::OnCommand(hwnd, id, hwndCtl, codeNotify);
    }
}

void CSineWnd::OnTimer(HWND hwnd, UINT id)
{
    switch (id)
    {
    case TIME_ID_PLAY:
    {
        if (!m_pAudioClient) break;

        BYTE *pData = NULL;
        UINT uFrameCount = 0, uFramesPadding = 0;
        HRESULT hr = m_pAudioClient->GetBufferSize(&uFrameCount);
        hr = m_pAudioClient->GetCurrentPadding(&uFramesPadding); // 还没有播放的帧数
        UINT uFill = uFrameCount - uFramesPadding;
        hr = m_pRenderClient->GetBuffer(uFill, &pData);
        float *pFloatData = reinterpret_cast<float*>(pData);
        for (unsigned int i = m_uFrameNow; i < m_uFrameNow + uFill; i++) {
            *pFloatData = sinf(400 * 2 * 3.1415927f*i / m_wfex.Format.nSamplesPerSec);
            *(pFloatData + 1) = *pFloatData;
            pFloatData += 2;
        }
        m_uFrameNow += uFill;
        hr = m_pRenderClient->ReleaseBuffer(uFill, 0);

        break;
    }
    default:
        break;
    }
}