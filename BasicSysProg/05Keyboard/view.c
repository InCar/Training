#include "stdafx.h"
#include "view.h"

void ViewOnPaint(View *this, HDC hdc)
{
	SetBkColor(hdc, 0);
	SetTextColor(hdc, this->color);
	SelectObject(hdc, this->fontText);
	SelectObject(hdc, this->hPen);

	RECT rc;
	GetClientRect(this->hWnd, &rc);
	rc.bottom = rc.top + this->pAPI->GetHeight(this);

	Model *pModel = this->pModel;
	FillRect(hdc, &rc, this->brBack);

	SetTextAlign(hdc, TA_CENTER);
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, (rc.left + rc.right) / 2, rc.top + this->nPadding, pModel->pAPI->GetString(pModel), pModel->pAPI->GetStringCount(pModel));

	// FPS
	SetTextAlign(hdc, TA_RIGHT);
	wchar_t wcsBuf[64];
	if (this->bNoLimit)
		wsprintf(wcsBuf, L"FPS: %4d", this->nFPS);
	else
		wsprintf(wcsBuf, L"FPS[%d MAX]: %4d", this->nMaxFPS, this->nFPS);
	TextOut(hdc, rc.right - this->nPadding, rc.top + this->nPadding, wcsBuf, (int)wcslen(wcsBuf));
	
	MoveToEx(hdc, rc.left, rc.bottom, NULL);
	LineTo(hdc, rc.right, rc.bottom);
}

int ViewGetHeight(View *this)
{
	TEXTMETRIC tm;
	HDC hdc = GetDC(this->hWnd);
	GetTextMetrics(hdc, &tm);
	ReleaseDC(this->hWnd, hdc);
	return tm.tmExternalLeading + tm.tmHeight + this->nPadding * 2;
}

void ViewChangeColor(View *this, COLORREF color)
{
	this->color = color;
	DeleteObject(this->hPen);
	this->hPen = CreatePen(PS_SOLID, 1, this->color);
}

void ViewSetFPS(View *this, int nFPS, int nMaxFPS, BOOL bNoLimit)
{
	this->nFPS = nFPS;
	this->nMaxFPS = nMaxFPS;
	this->bNoLimit = bNoLimit;
}

void ViewClose(View *this)
{
	DeleteObject(this->brBack);
	DeleteObject(this->fontText);
	DeleteObject(this->hPen);
}

View* ViewInit(View *this, Model *pModel, HWND hWnd)
{
	static ViewFunctions s_fns =
	{
		.OnPaint		= ViewOnPaint,
		.GetHeight		= ViewGetHeight,
		.ChangeColor	= ViewChangeColor,
		.SetFPS			= ViewSetFPS,
		.Close			= ViewClose
	};

	memset(this, 0, sizeof(View));
	this->pAPI = &s_fns;
	this->pModel = pModel;
	this->hWnd = hWnd;

	this->nPadding = 4;
	this->brBack = CreateSolidBrush(0); // black
	this->color = RGB(0x00, 0xff, 0x00); // green

	this->hPen = CreatePen(PS_SOLID, 1, this->color); // green

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	wcscpy_s(lf.lfFaceName, sizeof(lf.lfFaceName)/sizeof(wchar_t), L"ËÎÌו");
	lf.lfWeight = FW_BOLD;
	this->fontText = CreateFontIndirect(&lf);

	return this;
}