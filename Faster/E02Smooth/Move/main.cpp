#include "stdafx.h"
#include "MoveApp.h"

// Windows程序入口点
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrev, _In_ LPWSTR pCmdLine, _In_ int nCmdShow)
{
	// 创建MoveApp对象
	MoveApp *pMoveApp = new MoveApp();

	// 执行程序逻辑
	if (!pMoveApp->Init(hInstance)) return GetLastError();
	int nRet = pMoveApp->Run(pCmdLine, nCmdShow);
	pMoveApp->UnInit();

	// 清理MoveApp对象

	// 退出程序
	return nRet;
}