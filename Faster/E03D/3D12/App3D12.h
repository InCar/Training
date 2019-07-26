#pragma once
#include "MainWnd12.h"

class App3D12 : public App
{
public:
	App3D12();
	virtual ~App3D12();

	int Run(PWSTR pCmdLine, int nCmdShow) override;
private:
	MainWnd12 m_wndMain;
};

