#include "stdafx.h"
#include "model.h"

Point* ModelGetPoints(Model *pThis)
{
    return pThis->points;
}

int ModelGetCount(Model *pThis)
{
    return pThis->nCount;
}

Model* ModelClear(Model *pThis)
{
    pThis->nCount = 0;
    return pThis;
}

Model* ModelPut(Model *pThis, Point point)
{
    if (pThis->nCount < MAX_POINT_NUM) {
        pThis->points[pThis->nCount] = point;
        pThis->nCount++;
    }
    return pThis;
}

Model* ModelMove(Model *pThis, int nIndex, Point point)
{
    pThis->points[nIndex] = point;
    return pThis;
}

Model* ModelInit(Model *pThis)
{
    static ModelFunctions s_fns = {
        .GetPoints  = ModelGetPoints,
        .GetCount   = ModelGetCount,
        .Clear      = ModelClear,
        .Put        = ModelPut,
        .Move       = ModelMove
    };

    memset(pThis, 0, sizeof(Model));
    pThis->pAPI = &s_fns;
    return pThis;
}