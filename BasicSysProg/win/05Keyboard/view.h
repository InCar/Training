#pragma once
#include "model.h"

// 向前引用
typedef struct ViewFunctions ViewFunctions;

// 视图: 这是一个固定只有1行高的视图
typedef struct tagView
{
	Model		*pModel;
	HWND		hWnd;
	HPEN		hPen;
	HBRUSH		brBack;
	HFONT		fontText;
	COLORREF	color;
	int			nPadding;
	// API指针
	ViewFunctions	*pAPI;
} View;

// API定义
View* ViewInit(View*, Model*, HWND);

typedef void(*fnViewOnPaint)(View*, HDC hdc);
typedef int(*fnViewGetHeight)(View*);
typedef void(*fnViewChangeColor)(View*, COLORREF);
typedef void(*fnViewClose)(View*);

struct ViewFunctions
{
	fnViewOnPaint		OnPaint;
	fnViewGetHeight		GetHeight;
	fnViewChangeColor	ChangeColor;
	fnViewClose			Close;
};