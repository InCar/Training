#include "stdafx.h"
#include "HelloApp.h"

// Windows程序入口点
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ LPWSTR pCmdLine, _In_ int nCmdShow)
{
	// 创建HelloApp对象
	HelloApp app;

	// 执行程序逻辑
	if (!app.Init(hInstance)) return GetLastError();
	int nRet = app.Run(pCmdLine, nCmdShow);
	app.UnInit();
	
	// 退出程序
	return nRet;
}