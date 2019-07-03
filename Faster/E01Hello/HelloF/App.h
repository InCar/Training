#pragma once

// App基类定义
class App
{
public:
	// 方便获取App对象
	static App* GetApp() { return s_pApp; }

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
private:
	// 保持唯一的一个app对象的引用
	static App* s_pApp;
};

