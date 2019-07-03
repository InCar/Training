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
    HPEN        hPenSelect;
    HBRUSH      hBrush;
    HBRUSH      hBrushBk;
    COLORREF    colorBk;
    Point       pointDragStart;
    int         nDragPoint;
    int         nHoverPoint;
    int         nRange;
    RECT        rcSelect;
    BOOL        bShowSelect;
    BOOL        selected[MAX_POINT_NUM];
    Point       pointMoving[MAX_POINT_NUM];
    Point       pointMoveTo;
    float       fTheta;
    float       fSpeed;
    int         nDuration; // ms
    LARGE_INTEGER tmMoveBegin;
    LARGE_INTEGER freq;
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
typedef void(*fnViewMoveToPoint)(View*, Point);
typedef void(*fnViewMoveSpeed)(View*, float);
typedef BOOL(*fnViewMoving)(View*);

struct tagViewFunctions
{
    fnViewOnPaint		OnPaint;
    fnViewClose			Close;
    fnViewDragStart     DragStart;
    fnViewDragEnd       DragEnd;
    fnViewDragging      Dragging;
    fnViewHover         Hover;
    fnViewMoveToPoint   MoveToPoint;
    fnViewMoving        Moving;
    fnViewMoveSpeed     MoveSpeed;
};