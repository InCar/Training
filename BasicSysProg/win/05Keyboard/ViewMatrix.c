#include "stdafx.h"
#include "ViewMatrix.h"

void ExpandPoints(ViewMatrix*,int);

void ViewMatrixOnPaint(ViewMatrix *this, HDC hdc)
{
	SetBkColor(hdc, 0);
	SetTextColor(hdc, this->color);
	SelectObject(hdc, this->fontText);
	SelectObject(hdc, this->hPen);

	Model *pModel = this->pModel;
	FillRect(hdc, &this->rect, this->brBack);

	SetTextAlign(hdc, TA_CENTER);
	SetBkMode(hdc, TRANSPARENT);

	wchar_t *pBuf = pModel->pAPI->GetString(pModel);
	int nCount = pModel->pAPI->GetStringCount(pModel);

	// 如果有更多的字符,就需要扩展一下
	ExpandPoints(this, nCount);

	int nOffsetY = 18, nShadow = 12;

	for (int i = 0; i < nCount; i++) {
		for (int j = 0; j < nShadow; j++) {
			int y = this->pPoints[i].y + this->rect.top + this->nPadding - nOffsetY * j;
			if(y > this->rect.top)
				TextOut(hdc, this->pPoints[i].x, y, &pBuf[(y*nOffsetY) % nCount], 1);
		}
		this->pPoints[i].y += (1 + rand()%16);
		if (this->pPoints[i].y + this->rect.top + this->nPadding >= this->rect.bottom + nOffsetY*nShadow) this->pPoints[i].y = 0;
	}
}

void ViewMatrixChangeColor(ViewMatrix *this, COLORREF color)
{
	this->color = color;
	DeleteObject(this->hPen);
	this->hPen = CreatePen(PS_SOLID, 1, this->color);
}

void ViewMatrixSetRect(ViewMatrix *this, RECT *pRect)
{
	memcpy_s(&this->rect, sizeof(RECT), pRect, sizeof(RECT));

	// 变更范围
	int nWidth = pRect->right - pRect->left;
	ViewMatrixPoint *pPoint = this->pPoints;
	for (int i = 0; i < _msize(this->pPoints) / sizeof(ViewMatrixPoint); i++) {
		pPoint->x = rand() % nWidth;
		pPoint++;
	}
}

void ViewMatrixClose(ViewMatrix *this)
{
	DeleteObject(this->brBack);
	DeleteObject(this->fontText);
	DeleteObject(this->hPen);

	free(this->pPoints);
}

ViewMatrix* ViewMatrixInit(ViewMatrix *this, Model *pModel, HWND hWnd)
{
	static ViewMatrixFunctions s_fns =
	{
		.OnPaint		= ViewMatrixOnPaint,
		.ChangeColor	= ViewMatrixChangeColor,
		.SetRect		= ViewMatrixSetRect,
		.Close			= ViewMatrixClose
	};

	memset(this, 0, sizeof(ViewMatrix));
	this->pAPI = &s_fns;
	this->pModel = pModel;
	this->hWnd = hWnd;

	this->nPadding = 4;
	this->brBack = CreateSolidBrush(0); // black
	this->color = RGB(0x00, 0xff, 0x00); // green

	this->hPen = CreatePen(PS_SOLID, 1, this->color); // green

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	wcscpy_s(lf.lfFaceName, sizeof(lf.lfFaceName) / sizeof(wchar_t), L"宋体");
	lf.lfWeight = FW_BOLD;
	this->fontText = CreateFontIndirect(&lf);

	// 设置pPoints,每个字符对应一个点
	int nCount = pModel->pAPI->GetStringCount(pModel);
	if (nCount < 32) nCount = 32;
	this->pPoints = (ViewMatrixPoint*)malloc(sizeof(ViewMatrixPoint)*nCount);

	RECT rc;
	GetWindowRect(this->hWnd, &rc);
	int max = rc.right - rc.left;

	ViewMatrixPoint *pPoint = this->pPoints;
	memset(pPoint, 0, sizeof(ViewMatrixPoint)*nCount);
	srand(GetTickCount());
	for (int i = 0; i < nCount; i++) {
		pPoint->x = rand() % max;
		pPoint++;
	}

	return this;
}

void ExpandPoints(ViewMatrix *this, int nCount)
{
	int nOldCount = (int)(_msize(this->pPoints) / sizeof(ViewMatrixPoint));
	if (nCount > nOldCount) {
		// 需要扩展了
		int nDelta = (nCount - nOldCount)*2;
		if (nDelta < 32) nDelta = 32;

		this->pPoints = realloc(this->pPoints, sizeof(ViewMatrixPoint)*(nOldCount + nDelta));
		_ASSERT(this->pPoints != NULL);

		int nWidth = this->rect.right - this->rect.left;
		ViewMatrixPoint *pPoint = this->pPoints + nOldCount;
		memset(pPoint, 0, sizeof(ViewMatrixPoint)*nDelta);
		for (int i = 0; i < nDelta; i++) {
			pPoint->x = rand() % nWidth;
			pPoint++;
		}
	}
}