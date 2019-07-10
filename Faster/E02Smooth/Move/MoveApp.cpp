#include "stdafx.h"
#include "MoveApp.h"

MoveApp::MoveApp()
{
}

MoveApp::~MoveApp()
{
	
}

int MoveApp::Run(PWSTR pCmdLine, int nCmdShow)
{
	// 创建主窗口
	m_wndMain.Create();

	// 启动消息循环
	int nRet = App::Run(pCmdLine, nCmdShow);

	return nRet;
}
