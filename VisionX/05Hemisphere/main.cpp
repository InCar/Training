#include "stdafx.h"
#include "App.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
    CApp app;
    if (!app.Init(hInstance)) return GetLastError();
    int nRet = app.Run(pCmdLine, nCmdShow);
    app.UnInit();

    return nRet;
}