#pragma once
#include "XWnd.h"

class CMovPlayer : public CXWnd, IMFAsyncCallback
{
public:
    CMovPlayer();
    virtual ~CMovPlayer();
protected:
    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void OnRButtonUp(HWND hwnd, int x, int y, UINT flags);
    virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
    virtual void OnPaint(HWND hwnd);
    LRESULT OnPlayerEvent(MediaEventType meType, IMFMediaEvent *pMediaEvent);
private:
    IMFMediaSource *m_pSource;
    IMFMediaSession *m_pSession;
    // IMFVideoDisplayControl *m_pVideoDisplay;

    BOOL CMovPlayer::OpenMovFile(wstring &wstrMov);

    // IMFAsyncCallback
public:
    STDMETHODIMP QueryInterface(REFIID iid, void **ppv) { return E_NOTIMPL; };
    STDMETHODIMP_(ULONG) AddRef() { return 1; };
    STDMETHODIMP_(ULONG) Release() { return 1; };
    STDMETHODIMP GetParameters(DWORD *pdwFlags, DWORD *pdwQueue) { return E_NOTIMPL; }
    STDMETHODIMP Invoke(IMFAsyncResult* pAsyncResult);
};

