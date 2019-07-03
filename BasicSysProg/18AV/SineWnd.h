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
    virtual void OnTimer(HWND hwnd, UINT id);
    virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
    virtual void OnSize(HWND hwnd, UINT state, int cx, int cy);
    virtual HBRUSH OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, int type);
    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    UINT m_uFrameNow;
    HWND m_hwndTitle;
    HWND m_hwndBtnPlay;
    HWND m_hwndBtnStop;

    REFERENCE_TIME m_reftDuration;
    WAVEFORMATEXTENSIBLE m_wfex;
    IMMDevice *m_pMMDevice;
    IAudioClient *m_pAudioClient;
    IAudioRenderClient *m_pRenderClient;
};

