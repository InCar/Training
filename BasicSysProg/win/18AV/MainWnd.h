#pragma once
#include "XWnd.h"
#include "resource.h"

class CMainWnd : public CXWnd
{
public:
    CMainWnd();
    virtual ~CMainWnd();

    virtual ATOM Register();
    BOOL Create();
};

