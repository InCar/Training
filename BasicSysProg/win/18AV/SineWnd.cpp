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

    m_pAudioClient = NULL;
    m_pRenderClient = NULL;
}


CSineWnd::~CSineWnd()
{
}

BOOL CSineWnd::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    if (!CXWnd::OnCreate(hwnd, lpCreateStruct)) return FALSE;

    int width = 74, height = 24;
    int left = (lpCreateStruct->cx - width) / 2;
    int top = (lpCreateStruct->cy - height) / 2;
    m_hwnd = CreateWindow(L"Button", L"Play", WS_CHILD | WS_VISIBLE, left, top, width, height, hwnd, (HMENU)ID_AUDIO_PLAY, NULL, NULL);

    // 创建根对象设备枚举器
    IMMDeviceEnumerator *pIMMDeviceEnumerator = NULL;
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&pIMMDeviceEnumerator));

    // 获取默认播放设备
    IMMDevice *pMMDevice = NULL;
    hr = pIMMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pMMDevice);

    // 激活设备
    hr = pMMDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, reinterpret_cast<void**>(&m_pAudioClient));
    WAVEFORMATEX *pwfex = NULL;
    hr = m_pAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, reinterpret_cast<WAVEFORMATEX*>(&m_wfex), &pwfex);
    if (FAILED(hr)) {
        MessageBox(hwnd, L"不支持的音频格式-48KHz双声道16bits", L"Error", MB_OK);
        m_pAudioClient->Release();
        m_pAudioClient = NULL;

        pMMDevice->Release();
        pIMMDeviceEnumerator->Release();
        return FALSE;
    }

    hr = m_pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, m_reftDuration, 0, reinterpret_cast<WAVEFORMATEX*>(&m_wfex), NULL);
    hr = m_pAudioClient->GetService(__uuidof(IAudioRenderClient), reinterpret_cast<void**>(&m_pRenderClient));

    pMMDevice->Release();
    pIMMDeviceEnumerator->Release();

    return TRUE;
}

void CSineWnd::OnDestroy(HWND hwnd)
{
    m_pRenderClient->Release();
    m_pAudioClient->Release();

    CXWnd::OnDestroy(hwnd);
}

void CSineWnd::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case ID_AUDIO_PLAY:
    {
        UINT uFrameCount = 0;
        BYTE *pData = NULL;
        HRESULT hr = m_pAudioClient->GetBufferSize(&uFrameCount);
        hr = m_pRenderClient->GetBuffer(uFrameCount, &pData);
        // fill buffer
        float *pFloatData = reinterpret_cast<float*>(pData);
        for (unsigned int i = 0; i < uFrameCount; i++) {
            *pFloatData = sinf(400 * 2 * 3.1415927f*i / m_wfex.Format.nSamplesPerSec);
            *(pFloatData + 1) = *pFloatData;
            pFloatData += 2;
        }

        hr = m_pRenderClient->ReleaseBuffer(uFrameCount, 0);

        // 计算缓冲区可播放时间
        // double dDuration = (double)uFrameCount / m_wfex.Format.nSamplesPerSec;
        hr = m_pAudioClient->Start();
        Sleep(1000);
        hr = m_pAudioClient->Stop();
        break;
    }
    default:
        CXWnd::OnCommand(hwnd, id, hwndCtl, codeNotify);
    }
    
}