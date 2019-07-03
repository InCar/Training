#include "stdafx.h"
#include "AV.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
    CAV *pApp = dynamic_cast<CAV*>(CApp::GetApp());

    if (!pApp->Init(hInstance)) return GetLastError();
    int nRet = pApp->Run(pCmdLine, nCmdShow);
    pApp->UnInit();

    return nRet;
}