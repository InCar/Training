#pragma once
#include "model.h"

// 向前引用
typedef struct ViewMatrixFunctions ViewMatrixFunctions;

// 视图: 这是一个黑客帝国风格的视图
typedef struct tagViewMatrix
{
	Model		*pModel;
	HWND		hWnd;
	HPEN		hPen;
	HBRUSH		brBack;
	HFONT		fontText;
	COLORREF	color;
	int			nPadding;
	RECT		rect;
	POINT		*pPoints;
	// API指针
	ViewMatrixFunctions	*pAPI;
} ViewMatrix;

// API定义
ViewMatrix* ViewMatrixInit(ViewMatrix*, Model*, HWND);

typedef void(*fnViewMatrixOnPaint)(ViewMatrix*, HDC hdc);
typedef void(*fnViewMatrixChangeColor)(ViewMatrix*, COLORREF);
typedef void(*fnViewMatrixSetRect)(ViewMatrix*, RECT*);
typedef void(*fnViewMatrixClose)(ViewMatrix*);

struct ViewMatrixFunctions
{
	fnViewMatrixOnPaint		OnPaint;
	fnViewMatrixChangeColor	ChangeColor;
	fnViewMatrixClose		Close;
	fnViewMatrixSetRect		SetRect;
};