#pragma once
#include "MainWnd.h"

class CApp
{   
public:
    CApp();

    virtual BOOL Init(HINSTANCE hInstance);
    virtual int Run(PWSTR pCmdLine, int nCmdShow);
    virtual void UnInit();
private:
    HINSTANCE m_hInstance;
    ULONG_PTR m_token;
    std::unique_ptr<CMainWnd> m_uptrMainWnd;
};

