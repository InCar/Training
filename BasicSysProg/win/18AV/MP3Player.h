#pragma once
#include "XWnd.h"
class CMP3Player :
    public CXWnd
{
public:
    CMP3Player();
    virtual ~CMP3Player();
protected:
    virtual BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
    virtual void OnDestroy(HWND hwnd);
    virtual void OnSize(HWND hwnd, UINT state, int cx, int cy);
    virtual void OnTimer(HWND hwnd, UINT id);
    virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
    virtual HBRUSH OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, int type);
    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    HWND m_hwndTitle;
    HWND m_hwndBtnPlay;
    HWND m_hwndBtnStop;

    REFERENCE_TIME m_reftDuration;
    WAVEFORMATEX *m_pwfex;

    IMFSourceReader *m_pSourceReader;
    IMMDevice *m_pMMDevice;
    IAudioClient *m_pAudioClient;
    IAudioRenderClient *m_pRenderClient;
    IMFSample *m_pSample;

    IMFMediaBuffer *m_pLastBuffer;
    BYTE *m_pLastSrc;
    UINT m_uLastBufferSize;

    BOOL OpenMP3File(wstring &wstrMP3);
};

