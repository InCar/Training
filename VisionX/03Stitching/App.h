#pragma once
#include "MainWnd.h"

using namespace std;

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
    unique_ptr<CMainWnd> m_uptrMainWnd;
};

