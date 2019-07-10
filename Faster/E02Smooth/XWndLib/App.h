#pragma once

// App基类定义
class App
{
public:
	// 构造器
	App();
	// 析构器
	virtual ~App();

	// Windows程序实例
	HINSTANCE GetInstance() { return m_hInstance; }

	// 初始化
	virtual BOOL Init(HINSTANCE hInstance);
	// 主消息循环
	virtual int Run(PWSTR pCmdLine, int nCmdShow);
	// 反初始化
	virtual void UnInit();
protected:
	HINSTANCE m_hInstance;
};
