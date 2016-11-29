#pragma once
#include "App.h"
#include "MainWnd.h"

class CD3DApp : public CApp
{
public:
    CD3DApp();
    virtual ~CD3DApp();
    virtual int Run(PWSTR pCmdLine, int nCmdShow);
private:
    CMainWnd m_wndMain;
};