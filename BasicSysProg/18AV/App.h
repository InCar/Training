#pragma once

class CApp
{   
public:
    static CApp* GetApp() { return s_pApp; }

    CApp();
    virtual ~CApp();

    HINSTANCE GetInstance() { return m_hInstance; }

    virtual BOOL Init(HINSTANCE hInstance);
    virtual int Run(PWSTR pCmdLine, int nCmdShow);
    virtual void UnInit();
protected:
    HINSTANCE m_hInstance;
private:
    static CApp *s_pApp;
};

