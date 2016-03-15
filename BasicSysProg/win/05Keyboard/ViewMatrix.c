#include "stdafx.h"
#include "ViewMatrix.h"

void ExpandPoints(ViewMatrix *pView, int nCount);

void ViewMatrixOnPaint(ViewMatrix *pView, HDC hdc)
{
	SetBkColor(hdc, 0);
	SetTextColor(hdc, pView->color);
	SelectObject(hdc, pView->fontText);
	SelectObject(hdc, pView->hPen);

	Model *pModel = pView->pModel;
	FillRect(hdc, &pView->rect, pView->brBack);

	SetTextAlign(hdc, TA_CENTER);
	SetBkMode(hdc, TRANSPARENT);

	wchar_t *pBuf = pModel->pAPI->GetString(pModel);
	int nCount = pModel->pAPI->GetStringCount(pModel);

	// 如果有更多的字符,就需要扩展一下
	ExpandPoints(pView, nCount);

	int nOffsetY = 18, nShadow = 12;

	for (int i = 0; i < nCount; i++) {
		for (int j = 0; j < nShadow; j++) {
			int y = pView->pPoints[i].y + pView->rect.top + pView->nPadding - nOffsetY * j;
			if(y > pView->rect.top)
				TextOut(hdc, pView->pPoints[i].x, y, &pBuf[(y*nOffsetY) % nCount], 1);
		}
		pView->pPoints[i].y += (1 + rand()%16);
		if (pView->pPoints[i].y + pView->rect.top + pView->nPadding >= pView->rect.bottom + nOffsetY*nShadow) pView->pPoints[i].y = 0;
	}
}

void ViewMatrixChangeColor(ViewMatrix *pView, COLORREF color)
{
	pView->color = color;
	DeleteObject(pView->hPen);
	pView->hPen = CreatePen(PS_SOLID, 1, pView->color);
}

void ViewMatrixSetRect(ViewMatrix *pView, RECT *pRect)
{
	memcpy_s(&pView->rect, sizeof(RECT), pRect, sizeof(RECT));

	// 收缩范围
	int nWidth = pRect->right - pRect->left;
	POINT *pPoint = pView->pPoints;
	for (int i = 0; i < _msize(pView->pPoints) / sizeof(POINT); i++) {
		if (pPoint->x > nWidth) pPoint->x = pPoint->x % nWidth;
		pPoint++;
	}
}

void ViewMatrixClose(ViewMatrix *pView)
{
	DeleteObject(pView->brBack);
	DeleteObject(pView->fontText);
	DeleteObject(pView->hPen);

	free(pView->pPoints);
}

ViewMatrix* ViewMatrixInit(ViewMatrix *pView, Model *pModel, HWND hWnd)
{
	static ViewMatrixFunctions s_fns =
	{
		.OnPaint		= ViewMatrixOnPaint,
		.ChangeColor	= ViewMatrixChangeColor,
		.SetRect		= ViewMatrixSetRect,
		.Close			= ViewMatrixClose
	};

	memset(pView, 0, sizeof(ViewMatrix));
	pView->pAPI = &s_fns;
	pView->pModel = pModel;
	pView->hWnd = hWnd;

	pView->nPadding = 4;
	pView->brBack = CreateSolidBrush(0); // black
	pView->color = RGB(0x00, 0xff, 0x00); // green

	pView->hPen = CreatePen(PS_SOLID, 1, pView->color); // green

	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	wcscpy_s(lf.lfFaceName, sizeof(lf.lfFaceName) / sizeof(wchar_t), L"宋体");
	lf.lfWeight = FW_BOLD;
	pView->fontText = CreateFontIndirect(&lf);

	// 设置pPoints,每个字符对应一个点
	int nCount = pModel->pAPI->GetStringCount(pModel);
	if (nCount < 32) nCount = 32;
	pView->pPoints = (POINT*)malloc(sizeof(POINT)*nCount);

	int cx = GetSystemMetrics(SM_CXSCREEN);
	POINT *pPoint = pView->pPoints;
	memset(pPoint, 0, sizeof(POINT)*nCount);
	srand(GetTickCount());
	for (int i = 0; i < nCount; i++) {
		pPoint->x = rand() % cx;
		pPoint++;
	}

	return pView;
}

void ExpandPoints(ViewMatrix *pView, int nCount)
{
	int nOldCount = (int)(_msize(pView->pPoints) / sizeof(POINT));
	if (nCount > nOldCount) {
		// 需要扩展了
		int nDelta = (nCount - nOldCount)*2;
		if (nDelta < 32) nDelta = 32;

		pView->pPoints = realloc(pView->pPoints, sizeof(POINT)*(nOldCount + nDelta));
		_ASSERT(pView->pPoints != NULL);

		int nWidth = pView->rect.right - pView->rect.left;
		POINT *pPoint = pView->pPoints + nOldCount;
		memset(pPoint, 0, sizeof(POINT)*nDelta);
		for (int i = 0; i < nDelta; i++) {
			pPoint->x = rand() % nWidth;
			pPoint++;
		}
	}
}