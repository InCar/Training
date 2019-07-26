#include "stdafx.h"
#include "App3D11.h"

App3D11::App3D11()
{
}

App3D11::~App3D11()
{
}

int App3D11::Run(PWSTR pCmdLine, int nCmdShow)
{
	BOOL bOK = m_wndMain.Create();
	int nRet = __super::Run(pCmdLine, nCmdShow);

	if (!bOK) {
		DWORD dwError = GetLastError();
		MessageBox(NULL, L"´´½¨´°¿ÚÊ§°Ü", L"3D11", MB_TOPMOST | MB_OK);
	}

	return nRet;
}
