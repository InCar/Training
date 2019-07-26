#include "stdafx.h"
#include "App3D11.h"

App3D11 g_app3D11;

// Windows程序入口点
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ LPWSTR pCmdLine, _In_ int nCmdShow)
{
	g_app3D11.Init(hInstance);
	int nRet = g_app3D11.Run(pCmdLine, nCmdShow);
	g_app3D11.UnInit();

	return 0;
}