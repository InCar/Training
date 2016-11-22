#pragma once
#include "XWnd.h"
class CSineWnd :
    public CXWnd
{
public:
    CSineWnd();
    virtual ~CSineWnd();
protected:
    virtual BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
    virtual void OnDestroy(HWND hwnd);
    virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
private:
    HWND m_hwndBtnPlay;

    REFERENCE_TIME m_reftDuration;
    WAVEFORMATEXTENSIBLE m_wfex;
    IAudioClient *m_pAudioClient;
    IAudioRenderClient *m_pRenderClient;
};

