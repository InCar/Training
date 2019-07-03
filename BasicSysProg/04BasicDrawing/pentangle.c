#include "pentangle.h"
#include <math.h>

void calcVertexs(int xCenter, int yCenter, int radius, float fAngle, POINT *pVertexs, int count);

int Pentangle(HDC hdc, int xCenter, int yCenter, int radius, float fAngle)
{
	POINT vertexs[5];
	calcVertexs(xCenter, yCenter, radius, fAngle, vertexs, 5);

	int order[] = {0, 2, 4, 1, 3, 0};
	MoveToEx(hdc, vertexs[order[0]].x, vertexs[order[0]].y, NULL);
	for (int i = 1; i < sizeof(order)/sizeof(int); i++) {
		int t = order[i];
		LineTo(hdc, vertexs[t].x, vertexs[t].y);
	}

	return 0;
}

void calcVertexs(int xCenter, int yCenter, int radius, float fAngle, POINT *pVertexs, int count)
{
	const float fPI = 3.1415927f;
	float fAngVertex = 0;
	float fAngDelta = fPI*2.0f / count;

	for (int i = 0; i < count; i++) {
		fAngVertex = (fPI / 2.0f - fAngle) - fAngDelta * i;
		pVertexs[i].x = (int)(radius * cosf(fAngVertex)) + xCenter;
		pVertexs[i].y = (int)(radius * sinf(fAngVertex)) + yCenter;
	}
}