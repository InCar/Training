#include "stdafx.h"
#include "view.h"

void ViewOnPaint(View *pThis, HDC hdc)
{
    Model *pModel = pThis->pModel;
    int count = pModel->pAPI->GetCount(pModel);
    Point *points = pModel->pAPI->GetPoints(pModel);

    SelectObject(hdc, pThis->hPen);

    RECT rc;
    GetClientRect(pThis->hWnd, &rc);
    FillRect(hdc, &rc, GetStockObject(LTGRAY_BRUSH));

    int size = 8;
    if (pThis->nHoverPoint >= 0) {
        Point hover = points[pThis->nHoverPoint];
        Ellipse(hdc, hover.x - size, hover.y - size, hover.x + size, hover.y + size);
    }

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

void ViewDragStart(View *pThis, Point point)
{
    pThis->pointDragStart = point;

    // Hit Test
    Model *pModel = pThis->pModel;
    Point *points = pModel->pAPI->GetPoints(pModel);
    int count = pModel->pAPI->GetCount(pModel);

    for (int i = 0; i < count; i++) {
        if ((abs(points[i].x - point.x) < pThis->nRange) &&
            (abs(points[i].y - point.y) < pThis->nRange))
        {
            // tits point i 
            pThis->nDragPoint = i;
            return;
        }
    }

    // hits nothing
    pThis->nDragPoint = -1;
}

BOOL ViewDragEnd(View *pThis, Point point)
{
    pThis->nDragPoint = -1;
    // 任何一个方向上移动了nRange以上的距离视为拖动了
    if (abs(pThis->pointDragStart.x - point.x) < pThis->nRange &&
        abs(pThis->pointDragStart.y - point.y) < pThis->nRange)
    {
        return FALSE;
    }
    else
        return TRUE;
}

void ViewDragging(View *pThis, Point point)
{
    wchar_t buf[32];
    if (pThis->nDragPoint >= 0) {
        pThis->pModel->pAPI->Move(pThis->pModel, pThis->nDragPoint, point);
        
        wsprintf(buf, L"ddd\n", NULL);
    }
    else {
        wsprintf(buf, L"ccc\n", NULL);
    }

    OutputDebugString(buf);
}

void ViewHover(View *pThis, Point point)
{
    pThis->pointDragStart = point;

    // Hit Test
    Model *pModel = pThis->pModel;
    Point *points = pModel->pAPI->GetPoints(pModel);
    int count = pModel->pAPI->GetCount(pModel);

    for (int i = 0; i < count; i++) {
        if ((abs(points[i].x - point.x) < pThis->nRange) &&
            (abs(points[i].y - point.y) < pThis->nRange))
        {
            // tits point i 
            pThis->nHoverPoint = i;
            return;
        }
    }

    pThis->nHoverPoint = -1;
}

View* ViewInit(View *pThis, Model *pModel, HWND hWnd)
{
    static ViewFunctions s_fns =
    {
        .OnPaint = ViewOnPaint,
        .Close = ViewClose,
        .DragStart = ViewDragStart,
        .DragEnd = ViewDragEnd,
        .Dragging = ViewDragging,
        .Hover = ViewHover
    };

    memset(pThis, 0, sizeof(View));
    pThis->pAPI = &s_fns;
    pThis->pModel = pModel;
    pThis->hWnd = hWnd;
    pThis->nDragPoint = -1;
    pThis->nHoverPoint = -1;
    pThis->nRange = 8;

    pThis->hPen = CreatePen(PS_SOLID, 1, RGB(0xcc, 0x00, 0x00)); // red

    return pThis;
}