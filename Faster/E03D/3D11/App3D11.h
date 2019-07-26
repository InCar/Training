#pragma once
#include "MainWnd11.h"

class App3D11 : public App
{
public:
	App3D11();
	virtual ~App3D11();

	int Run(PWSTR pCmdLine, int nCmdShow) override;
private:
	MainWnd11 m_wndMain;
};

