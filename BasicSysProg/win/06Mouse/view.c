#include "stdafx.h"
#include "view.h"

void ViewOnPaint(View *pThis, HDC hdc)
{
    Model *pModel = pThis->pModel;
    int count = pModel->pAPI->GetCount(pModel);
    Point *points = pModel->pAPI->GetPoints(pModel);

    SelectObject(hdc, pThis->hPen);

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (i == j) continue; // skip the same points
            MoveToEx(hdc, points[i].x, points[i].y, NULL);
            LineTo(hdc, points[j].x, points[j].y);
        }
    }
}

void ViewClose(View *pThis)
{
    DeleteObject(pThis->hPen);
}

View* ViewInit(View *pThis, Model *pModel, HWND hWnd)
{
    static ViewFunctions s_fns =
    {
        .OnPaint = ViewOnPaint,
        .Close = ViewClose
    };

    memset(pThis, 0, sizeof(View));
    pThis->pAPI = &s_fns;
    pThis->pModel = pModel;
    pThis->hWnd = hWnd;

    pThis->hPen = CreatePen(PS_SOLID, 1, RGB(0xcc, 0x00, 0x00)); // red

    return pThis;
}