#pragma once
#include "App.h"
#include "MainWnd.h"

using namespace std;

class CAV : public CApp
{
public:
    CAV();
    virtual ~CAV();
    
    virtual int Run(PWSTR pCmdLine, int nCmdShow);
private:
    CMainWnd m_wndMain;
};