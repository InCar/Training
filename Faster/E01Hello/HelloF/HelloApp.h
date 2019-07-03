#pragma once
#include "App.h"
#include "MainWnd.h"

class HelloApp : public App
{
public:
	HelloApp();
	~HelloApp() override;

	int Run(PWSTR pCmdLine, int nCmdShow) override;
private:
	// 主窗口对象
	MainWnd m_wndMain;
};

