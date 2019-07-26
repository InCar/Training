#include "stdafx.h"
#include "App3D12.h"

App3D12::App3D12()
{
}

App3D12::~App3D12()
{
}

int App3D12::Run(PWSTR pCmdLine, int nCmdShow)
{
	BOOL bOK = m_wndMain.Create();
	int nRet = __super::Run(pCmdLine, nCmdShow);

	if (!bOK) {
		DWORD dwError = GetLastError();
		MessageBox(NULL, L"´´½¨´°¿ÚÊ§°Ü", L"3D", MB_TOPMOST | MB_OK);
	}

	return nRet;
}
