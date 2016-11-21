#include "stdafx.h"
#include "SineWnd.h"
#include "resource.h"

CSineWnd::CSineWnd()
{
    m_reftDuration = 10000000; // in 100-nano-sec
    m_pwfex = NULL;
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

    return TRUE;
}

void CSineWnd::OnDestroy(HWND hwnd)
{
    CoTaskMemFree(m_pwfex);
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
        // 创建根对象设备枚举器
        IMMDeviceEnumerator *pIMMDeviceEnumerator = NULL;
        HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&pIMMDeviceEnumerator));

        // 获取默认播放设备
        IMMDevice *pMMDevice = NULL;
        hr = pIMMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pMMDevice);

        // 激活设备
        UINT32 uFrameCount = 0;
        hr = pMMDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, reinterpret_cast<void**>(&m_pAudioClient));
        hr = m_pAudioClient->GetMixFormat(&m_pwfex);
        hr = m_pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, m_reftDuration, 0, m_pwfex, NULL);
        hr = m_pAudioClient->GetBufferSize(&uFrameCount);
        hr = m_pAudioClient->GetService(__uuidof(IAudioRenderClient), reinterpret_cast<void**>(&m_pRenderClient));

        BYTE *pData;
        hr = m_pRenderClient->GetBuffer(uFrameCount, &pData);
        // TODO: fill buffer

        hr = m_pRenderClient->ReleaseBuffer(uFrameCount, 0);

        // 计算缓冲区可播放时间
        double dDuration = (double)uFrameCount / m_pwfex->nSamplesPerSec;

        hr = m_pAudioClient->Start();

        pMMDevice->Release();
        pIMMDeviceEnumerator->Release();
        break;
    }
    default:
        CXWnd::OnCommand(hwnd, id, hwndCtl, codeNotify);
    }
    
}