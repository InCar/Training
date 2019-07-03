#include "stdafx.h"
#include "HelloApp.h"

HelloApp::HelloApp()
{
}

HelloApp::~HelloApp()
{
}

int HelloApp::Run(PWSTR pCmdLine, int nCmdShow)
{
	// 创建主窗口对象
	m_wndMain.Create();

	// 基类的主消息循环
	return App::Run(pCmdLine, nCmdShow);
}
