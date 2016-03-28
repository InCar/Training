#pragma once
#include "model.h"

// 向前引用
typedef struct tagViewFunctions ViewFunctions;

// 视图: 这是一个固定只有1行高的视图
typedef struct tagView
{
    Model		*pModel;
    HWND		hWnd;
    HPEN		hPen;
    // API指针
    ViewFunctions	*pAPI;
} View;

// API定义
View* ViewInit(View*, Model*, HWND);

typedef void(*fnViewOnPaint)(View*, HDC hdc);
typedef void(*fnViewClose)(View*);

struct tagViewFunctions
{
    fnViewOnPaint		OnPaint;
    fnViewClose			Close;
};