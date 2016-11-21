#pragma once
#include "XWnd.h"
#include "resource.h"
#include "SineWnd.h"

class CMainWnd : public CXWnd
{
public:
    CMainWnd();
    virtual ~CMainWnd();

    virtual ATOM Register();
    BOOL Create();
private:
    CSineWnd m_wndSine;
};

