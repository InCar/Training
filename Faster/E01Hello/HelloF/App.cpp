#include "stdafx.h"
#include "App.h"

// 保持唯一的一个app对象的引用
App* App::s_pApp = NULL;

App::App()
{
	// 确保同一时间仅有一个App对象
	assert(s_pApp == NULL);
	s_pApp = this;

	m_hInstance = NULL;
}


App::~App()
{
	s_pApp = NULL;
}

BOOL App::Init(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	return TRUE;
}

void App::UnInit()
{
}

int App::Run(PWSTR pCmdLine, int nCmdShow)
{
	// 主消息循环
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}