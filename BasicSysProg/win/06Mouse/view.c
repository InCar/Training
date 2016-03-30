#include "stdafx.h"
#include "view.h"

inline BOOL PointsInRange(Point p1, Point p2, int range)
{
    return (abs(p1.x - p2.x) < range) && (abs(p1.y - p2.y) < range);
}

inline BOOL PointInRectangle(Point p, RECT *pRect)
{
    return (((p.x - pRect->left) * (p.x - pRect->right)) < 0) &&
        (((p.y - pRect->top) * (p.y - pRect->bottom)) < 0);
}

void ViewOnPaint(View *pThis, HDC hdc)
{
    Model *pModel = pThis->pModel;
    int count = pModel->pAPI->GetCount(pModel);
    Point *points = pModel->pAPI->GetPoints(pModel);

    SetBkColor(hdc, pThis->colorBk);
    SelectObject(hdc, pThis->hPen);
    SelectObject(hdc, pThis->hBrushBk);

    // Background
    RECT rc;
    GetClientRect(pThis->hWnd, &rc);
    FillRect(hdc, &rc, pThis->hBrushBk);

    // Hover circle
    int size = pThis->nRange;
    if (pThis->nHoverPoint >= 0) {
        Point hover = points[pThis->nHoverPoint];
        Ellipse(hdc, hover.x - size, hover.y - size, hover.x + size, hover.y + size);
    }

    // Points
    SelectObject(hdc, pThis->hBrush);
    size /= 2;
    for (int i = 0; i < count; i++) {
        Ellipse(hdc, points[i].x - size, points[i].y - size, points[i].x + size, points[i].y + size);
    }
    // selected
    SelectObject(hdc, GetStockBrush(NULL_BRUSH));
    for (int i = 0; i < count; i++) {
        if (pThis->selected[i])
            Ellipse(hdc, points[i].x - size * 2, points[i].y - size * 2, points[i].x + size * 2, points[i].y + size * 2);
    }

    // Lines
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (i == j) continue; // skip the same points
            MoveToEx(hdc, points[i].x, points[i].y, NULL);
            LineTo(hdc, points[j].x, points[j].y);
        }
    }

    if (pThis->bShowSelect) {
        // Select Box
        SelectObject(hdc, pThis->hPenSelect);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, pThis->rcSelect.left, pThis->rcSelect.top, pThis->rcSelect.right, pThis->rcSelect.bottom);
    }

    if (pThis->pointMoveTo.x && pThis->pointMoveTo.y) {
        if(pThis->pAPI->Moving(pThis)) InvalidateRect(pThis->hWnd, NULL, FALSE);
    }
}

void ViewClose(View *pThis)
{
    DeleteObject(pThis->hPen);
    DeleteObject(pThis->hPenSelect);
    DeleteObject(pThis->hBrush);
    DeleteObject(pThis->hBrushBk);
}

void ViewDragStart(View *pThis, Point point)
{
    pThis->pointDragStart = point;

    // Hit Test
    Model *pModel = pThis->pModel;
    Point *points = pModel->pAPI->GetPoints(pModel);
    int count = pModel->pAPI->GetCount(pModel);

    for (int i = 0; i < count; i++) {
        if(PointsInRange(points[i], point, pThis->nRange))
        {
            // tits point i 
            pThis->nDragPoint = i;
            return;
        }
    }

    // hits nothing
    pThis->nDragPoint = -1;
    // Select Box
    pThis->bShowSelect = TRUE;
    pThis->rcSelect.left = point.x;
    pThis->rcSelect.top = point.y;
    pThis->rcSelect.right = point.x;
    pThis->rcSelect.bottom = point.y;
}

BOOL ViewDragEnd(View *pThis, Point point)
{
    // SELECTED
    if (pThis->bShowSelect) {
        Model *pModel = pThis->pModel;
        Point *points = pModel->pAPI->GetPoints(pModel);
        int count = pModel->pAPI->GetCount(pModel);

        for (int i = 0; i < count; i++) {
            if (PointInRectangle(points[i], &pThis->rcSelect))
                pThis->selected[i] = TRUE;
            else
                pThis->selected[i] = FALSE;
        }
    }

    pThis->nDragPoint = -1;
    pThis->bShowSelect = FALSE;
    // 任何一个方向上移动了nRange以上的距离视为拖动了
    if(PointsInRange(pThis->pointDragStart, point, pThis->nRange))
    {
        return FALSE;
    }
    else
        return TRUE;
}

void ViewDragging(View *pThis, Point point)
{
    if (pThis->nDragPoint >= 0) {
        pThis->pModel->pAPI->Move(pThis->pModel, pThis->nDragPoint, point);
    }
    else {
        pThis->rcSelect.right = point.x;
        pThis->rcSelect.bottom = point.y;
    }
}

void ViewHover(View *pThis, Point point)
{
    pThis->pointDragStart = point;

    // Hit Test
    Model *pModel = pThis->pModel;
    Point *points = pModel->pAPI->GetPoints(pModel);
    int count = pModel->pAPI->GetCount(pModel);

    for (int i = 0; i < count; i++) {
        if(PointsInRange(points[i], point, pThis->nRange))
        {
            // tits point i 
            pThis->nHoverPoint = i;
            return;
        }
    }

    pThis->nHoverPoint = -1;
}

void ViewMoveToPoint(View *pThis, Point point)
{
    pThis->pointMoveTo = point;

    Model *pModel = pThis->pModel;
    Point *points = pModel->pAPI->GetPoints(pModel);
    int count = pModel->pAPI->GetCount(pModel);

    Point pO = { .x = 0, .y = 0 };
    int cSel = 0;
    for (int i = 0; i < count; i++) {
        if (pThis->selected[i]) {
            pO.x += points[i].x;
            pO.y += points[i].y;
            pThis->pointMoving[i] = points[i];
            cSel++;
        }
    }

    if (cSel > 0) {
        pO.x /= cSel;
        pO.y /= cSel;

        pThis->fTheta = atan2f((float)point.x - pO.x, (float)point.y - pO.y);
        QueryPerformanceCounter(&pThis->tmMoveBegin);
        pThis->nDuration = (int)((point.x - pO.x) / (sinf(pThis->fTheta)*pThis->fSpeed));
        InvalidateRect(pThis->hWnd, NULL, FALSE);
    }
}

void ViewMoveSpeed(View *pThis, float fSpeed)
{
    pThis->fSpeed = fSpeed;
    pThis->pAPI->MoveToPoint(pThis, pThis->pointMoveTo);
}

BOOL ViewMoving(View *pThis)
{
    LARGE_INTEGER tmNow;
    QueryPerformanceCounter(&tmNow);
    int past = (int)((tmNow.QuadPart - pThis->tmMoveBegin.QuadPart)*1000 / pThis->freq.QuadPart);
    if (past > pThis->nDuration) return FALSE;

    Model *pModel = pThis->pModel;
    Point *points = pModel->pAPI->GetPoints(pModel);
    int count = pModel->pAPI->GetCount(pModel);
    for (int i = 0; i < count; i++) {
        if (pThis->selected[i]) {
            points[i].x = (int)(pThis->fSpeed*past*sinf(pThis->fTheta)) + pThis->pointMoving[i].x;
            points[i].y = (int)(pThis->fSpeed*past*cosf(pThis->fTheta)) + pThis->pointMoving[i].y;
        }
    }
    return TRUE;
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
        .Hover = ViewHover,
        .MoveToPoint = ViewMoveToPoint,
        .Moving = ViewMoving,
        .MoveSpeed = ViewMoveSpeed
    };

    memset(pThis, 0, sizeof(View));
    pThis->pAPI = &s_fns;
    pThis->pModel = pModel;
    pThis->hWnd = hWnd;
    pThis->nDragPoint = -1;
    pThis->nHoverPoint = -1;
    pThis->nRange = 8;
    pThis->fSpeed = 2.0f;

    QueryPerformanceFrequency(&pThis->freq);

    COLORREF color = RGB(0xcc, 0x00, 0x00);
    pThis->colorBk = RGB(0xcc, 0xcc, 0xcc); // gray
    pThis->hPen = CreatePen(PS_SOLID, 1, color); // red
    pThis->hPenSelect = CreatePen(PS_DOT, 1, color);
    pThis->hBrush = CreateSolidBrush(color);
    pThis->hBrushBk = CreateSolidBrush(pThis->colorBk);

    return pThis;
}