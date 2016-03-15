#include "stdafx.h"
#include "view.h"

void ViewOnPaint(View *pView, HDC hdc)
{
	SetBkColor(hdc, 0);
	SetTextColor(hdc, pView->color);
	SelectObject(hdc, pView->fontText);
	SelectObject(hdc, pView->hPen);

	RECT rc;
	GetClientRect(pView->hWnd, &rc);
	int nYLine = pView->pAPI->GetHeight(pView) + pView->nPadding * 2;
	rc.bottom = rc.top + nYLine;

	Model *pModel = pView->pModel;
	FillRect(hdc, &rc, pView->brBack);

	SetTextAlign(hdc, TA_CENTER);
	TextOut(hdc, rc.right/2, pView->nPadding, pModel->pAPI->GetString(pModel), pModel->pAPI->GetStringCount(pModel));
	
	MoveToEx(hdc, rc.left, rc.bottom, NULL);
	LineTo(hdc, rc.right, rc.bottom);
}

int ViewGetHeight(View *pView)
{
	TEXTMETRIC tm;
	HDC hdc = GetDC(pView->hWnd);
	GetTextMetrics(hdc, &tm);
	ReleaseDC(pView->hWnd, hdc);
	return tm.tmExternalLeading + tm.tmHeight;
}

void ViewClose(View *pView)
{
	DeleteObject(pView->brBack);
	DeleteObject(pView->fontText);
	DeleteObject(pView->hPen);
}

void ViewChangeColor(View *pView, COLORREF color)
{
	pView->color = color;
	DeleteObject(pView->hPen);
	pView->hPen = CreatePen(PS_SOLID, 1, pView->color);
}

View* ViewInit(View *pView, Model *pModel, HWND hWnd)
{
	static ViewFunctions s_fns =
	{
		.OnPaint		= ViewOnPaint,
		.GetHeight		= ViewGetHeight,
		.Close			= ViewClose,
		.ChangeColor	= ViewChangeColor
	};

	memset(pView, 0, sizeof(View));
	pView->pAPI = &s_fns;
	pView->pModel = pModel;
	pView->hWnd = hWnd;

	pView->nPadding = 4;
	pView->brBack = CreateSolidBrush(0); // black
	pView->color = RGB(0x00, 0xff, 0x00); // green

	pView->hPen = CreatePen(PS_SOLID, 1, pView->color); // green

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	wcscpy_s(lf.lfFaceName, sizeof(lf.lfFaceName)/sizeof(wchar_t), L"ËÎÌו");
	lf.lfWeight = FW_BOLD;
	pView->fontText = CreateFontIndirect(&lf);

	return pView;
}