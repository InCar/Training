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
    Point       pointDragStart;
    int         nDragPoint;
    int         nHoverPoint;
    int         nRange;
    // API指针
    ViewFunctions	*pAPI;
} View;

// API定义
View* ViewInit(View*, Model*, HWND);

typedef void(*fnViewOnPaint)(View*, HDC hdc);
typedef void(*fnViewClose)(View*);
typedef void(*fnViewDragStart)(View*, Point);
typedef BOOL(*fnViewDragEnd)(View*, Point);
typedef void(*fnViewDragging)(View*, Point);
typedef void(*fnViewHover)(View*, Point);

struct tagViewFunctions
{
    fnViewOnPaint		OnPaint;
    fnViewClose			Close;
    fnViewDragStart     DragStart;
    fnViewDragEnd       DragEnd;
    fnViewDragging      Dragging;
    fnViewHover         Hover;
};