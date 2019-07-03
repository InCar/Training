#include "stdafx.h"
#include "D3DApp.h"

CD3DApp g_app;

CD3DApp::CD3DApp()
{
}


CD3DApp::~CD3DApp()
{
}

int CD3DApp::Run(PWSTR pCmdLine, int nCmdShow)
{
    m_wndMain.Create();

    return CApp::Run(pCmdLine, nCmdShow);
}