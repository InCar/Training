#pragma once
#include "XWnd.h"
class CMovPlayer : public CXWnd
{
public:
    CMovPlayer();
    virtual ~CMovPlayer();
protected:
    virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void OnRButtonUp(HWND hwnd, int x, int y, UINT flags);
    virtual void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
private:
    BOOL CMovPlayer::OpenMovFile(wstring &wstrMov);
};

