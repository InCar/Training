#pragma once
class MainWnd : public XWnd
{
public:
	MainWnd();
	virtual ~MainWnd();

	BOOL Create();
protected:
		ATOM Register();
};

