#include "stdafx.h"
#include "AV.h"

CAV g_app;

CAV::CAV()
{
}


CAV::~CAV()
{
}

int CAV::Run(PWSTR pCmdLine, int nCmdShow)
{
    m_wndMain.Create();

    return CApp::Run(pCmdLine, nCmdShow);
}