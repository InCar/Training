#include "stdafx.h"
#include "App.h"

App::App()
{
	m_hInstance = NULL;
}


App::~App()
{
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