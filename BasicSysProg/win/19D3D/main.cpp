#include "stdafx.h"
#include "D3DApp.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
    CD3DApp *pApp = dynamic_cast<CD3DApp*>(CApp::GetApp());

    if (!pApp->Init(hInstance)) return GetLastError();
    int nRet = pApp->Run(pCmdLine, nCmdShow);
    pApp->UnInit();

    return nRet;
}