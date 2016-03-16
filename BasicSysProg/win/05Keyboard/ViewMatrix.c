#include "stdafx.h"
#include "ViewMatrix.h"

void ExpandPoints(ViewMatrix*);
void ReAssignXZ(ViewMatrix*, ViewMatrixPoint*, int);

void ViewMatrixOnPaint(ViewMatrix *this, HDC hdc)
{
	SetBkColor(hdc, 0);
	SetTextColor(hdc, this->color);
	SelectObject(hdc, this->fontText);
	SelectObject(hdc, this->hPen);

	HRGN hClip = CreateRectRgnIndirect(&this->rect);
	SelectClipRgn(hdc, hClip);

	Model *pModel = this->pModel;
	FillRect(hdc, &this->rect, this->brBack);

	SetTextAlign(hdc, TA_CENTER);
	SetBkMode(hdc, TRANSPARENT);

	wchar_t *pBuf = pModel->pAPI->GetString(pModel);
	int nCount = pModel->pAPI->GetStringCount(pModel);

	// 如果有更多的字符,就需要扩展一下
	ExpandPoints(this);

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);
	int nLineHeight = /*tm.tmExternalLeading +*/ tm.tmHeight;
	DWORD dwTickNow = GetTickCount();
	int nFontCount = sizeof(this->fontTexts) / sizeof(HFONT);

	ViewMatrixPoint *pPoint = this->pPoints;
	for (int i = 0; i < nCount; i++) {
		int nMaxY = this->rect.bottom + nLineHeight * pPoint->nShadow;

		for (int j = 0; j < pPoint->nShadow; j++) {
			// 轨迹渐淡
			BYTE color[3] = { GetRValue(this->color) , GetGValue(this->color) , GetBValue(this->color) };
			for (int c = 0; c < 3; c++) {
				color[c] = color[c] - color[c] * j / (pPoint->nShadow*2);
				// Z
				color[c] = color[c] - color[c] * pPoint->z / (this->nMaxZ * 2);
			}
			SetTextColor(hdc, RGB(color[0], color[1], color[2]));
			// 字体大小
			int nFontIndex = nFontCount*pPoint->z / this->nMaxZ;
			SelectObject(hdc, this->fontTexts[nFontIndex]);
			GetTextMetrics(hdc, &tm);
			int nLineHeight = /*tm.tmExternalLeading +*/ tm.tmHeight;

			TextOut(hdc, pPoint->x, pPoint->y - nLineHeight*j + this->rect.top, pBuf + ((pPoint->nCharPos + j) % nCount), 1);
		}

		// 下坠计算
		int ms = dwTickNow - pPoint->dwTick;
		int nDeltaY = (int)(pPoint->fSpeed*ms / 1000.0f);
		if (nDeltaY > 0) {
			pPoint->y += nDeltaY;
			if (pPoint->y > nMaxY) {
				pPoint->y = 0;
				ReAssignXZ(this, pPoint, 1);
			}
			pPoint->dwTick = dwTickNow;
		}

		// 变换字符计算
		ms = dwTickNow - pPoint->dwCharTick;
		if (ms > 200) {
			pPoint->nCharPos = rand();
			pPoint->dwCharTick = dwTickNow;
		}

		pPoint++;
	}

	SelectClipRgn(hdc, NULL);
	DeleteObject(hClip);


	// int nOffsetY = 18, nShadow = 12;
	// for (int i = 0; i < nCount; i++) {
	// 	for (int j = 0; j < nShadow; j++) {
	// 		int y = this->pPoints[i].y + this->rect.top + this->nPadding - nOffsetY * j;
	// 		if(y > this->rect.top)
	// 			TextOut(hdc, this->pPoints[i].x, y, &pBuf[(y*nOffsetY) % nCount], 1);
	// 	}
	// 	this->pPoints[i].y += (1 + rand()%16);
	// 	if (this->pPoints[i].y + this->rect.top + this->nPadding >= this->rect.bottom + nOffsetY*nShadow) this->pPoints[i].y = 0;
	// }
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

	this->nMaxX = pRect->right - pRect->left;
	this->nMaxZ = 16;

	// 变更范围
	ReAssignXZ(this, this->pPoints, (int)_msize(this->pPoints) / sizeof(ViewMatrixPoint));
}

void ViewMatrixClose(ViewMatrix *this)
{
	DeleteObject(this->brBack);
	DeleteObject(this->fontText);
	for (int i = 0; i < sizeof(this->fontTexts) / sizeof(HFONT); i++)
		DeleteObject(this->fontTexts[i]);
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
	for (int i = 0; i < sizeof(this->fontTexts) / sizeof(HFONT); i++) {
		lf.lfHeight = 24 - 24 * i / (sizeof(this->fontTexts) / sizeof(HFONT) * 2);
		this->fontTexts[i] = CreateFontIndirect(&lf);
	}

	// 设置pPoints,每个字符对应一个点
	int nCount = pModel->pAPI->GetStringCount(pModel);
	if (nCount < 32) nCount = 32;
	this->pPoints = (ViewMatrixPoint*)malloc(sizeof(ViewMatrixPoint)*nCount);
	memset(this->pPoints, 0, sizeof(ViewMatrixPoint)*nCount);

	return this;
}

void ExpandPoints(ViewMatrix *this)
{
	int nOldCount = (int)(_msize(this->pPoints) / sizeof(ViewMatrixPoint));
	int nNewCount = this->pModel->pAPI->GetStringCount(this->pModel);

	if (nNewCount > nOldCount) {
		// 需要扩展了
		int nDelta = (nNewCount - nOldCount)*2;
		if (nDelta < 32) nDelta = 32;

		this->pPoints = realloc(this->pPoints, sizeof(ViewMatrixPoint)*(nOldCount + nDelta));
		_ASSERT(this->pPoints != NULL);

		ViewMatrixPoint *pPoints = this->pPoints + nOldCount;
		memset(pPoints, 0, sizeof(ViewMatrixPoint)*nDelta);
		ReAssignXZ(this, pPoints, nDelta);
	}
}

void ReAssignXZ(ViewMatrix *this, ViewMatrixPoint *pPoints, int nCount)
{
	int nMinShadow = 4, nMaxShadow = 16;
	float fMinSpeed = 96.0f, fMaxSpeed = 620.0f;
	DWORD dwTick = GetTickCount();

	for (int i = 0; i < nCount; i++) {
		pPoints->x = rand() % this->nMaxX;
		pPoints->z = rand() % this->nMaxZ;
		pPoints->nCharPos = rand();
		pPoints->nShadow = (rand() % (nMaxShadow - nMinShadow)) + nMinShadow;
		pPoints->fSpeed = ((float)rand() / RAND_MAX)*(fMaxSpeed - fMinSpeed) + fMinSpeed;
		pPoints->dwTick = dwTick;
		pPoints->dwCharTick = dwTick;

		pPoints++;
	}
}