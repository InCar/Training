#pragma once

// 向前引用
typedef struct tagSimpleWndFunctions SimpleWndFunctions;

// 子窗口
typedef struct tagSimpleWnd
{
    HWND hwnd;
    // API指针
    SimpleWndFunctions	*pAPI;
} SimpleWnd;

// API定义
SimpleWnd* SimpleWndInit(SimpleWnd*);

typedef void(*fnSimpleWndCreate)(SimpleWnd*, HWND, const RECT*, const wchar_t*);

struct tagSimpleWndFunctions
{
    wchar_t                 *pwszClsName;
    fnSimpleWndCreate       Create;
};