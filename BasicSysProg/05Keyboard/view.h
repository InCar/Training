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
	HBRUSH		brBack;
	HFONT		fontText;
	COLORREF	color;
	int			nPadding;
	int			nFPS;
	int			nMaxFPS;
	BOOL		bNoLimit;
	// API指针
	ViewFunctions	*pAPI;
} View;

// API定义
View* ViewInit(View*, Model*, HWND);

typedef void(*fnViewOnPaint)(View*, HDC hdc);
typedef int(*fnViewGetHeight)(View*);
typedef void(*fnViewChangeColor)(View*, COLORREF);
typedef void(*fnViewSetFPS)(View*, int, int, BOOL);
typedef void(*fnViewClose)(View*);

struct tagViewFunctions
{
	fnViewOnPaint		OnPaint;
	fnViewGetHeight		GetHeight;
	fnViewChangeColor	ChangeColor;
	fnViewSetFPS		SetFPS;
	fnViewClose			Close;
};