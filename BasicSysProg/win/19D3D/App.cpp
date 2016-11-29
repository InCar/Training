#include "StdAfx.h"
#include "App.h"

CApp* CApp::s_pApp = NULL;

CApp::CApp()
{
    s_pApp = this;

    m_hInstance = NULL;
}


CApp::~CApp()
{
}

BOOL CApp::Init(HINSTANCE hInstance)
{
    m_hInstance = hInstance;

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) return FALSE;

    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);

    return TRUE;
}

void CApp::UnInit()
{
    CoUninitialize();
}

int CApp::Run(PWSTR pCmdLine, int nCmdShow)
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}
