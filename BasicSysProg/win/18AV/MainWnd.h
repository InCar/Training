#pragma once
#include "XWnd.h"
#include "resource.h"
#include "SineWnd.h"
#include "MP3Player.h"
#include "MovPlayer.h"

class CMainWnd : public CXWnd
{
public:
    CMainWnd();
    virtual ~CMainWnd();

    virtual ATOM Register();
    BOOL Create();
protected:
    virtual void OnSize(HWND hwnd, UINT state, int cx, int cy);
private:
    CSineWnd m_wndSine;
    CMP3Player m_wndMP3Player;
    CMovPlayer m_wndMovPlayer;
};

