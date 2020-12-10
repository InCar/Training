#include "stdafx.h"
#include "OpticaFisheyeSin.h"

using namespace std;

OpticaFisheyeSin::OpticaFisheyeSin()
	: Optica(38.0f)
{
}

OpticaFisheyeSin::OpticaFisheyeSin(float f135)
	: Optica(f135)
{
}

float OpticaFisheyeSin::CalcRFromTheta(float fTheta, int width)
{
	// 焦距(以像素为单位)
	float fFocus = width * m_f135 / c_fWidthFullFrame;

	// 正弦鱼眼镜头 r = f * sin(theta)
	return fFocus * sinf(fTheta);
}
