#include "stdafx.h"
#include "OpticaFisheyeArea.h"

using namespace std;

OpticaFisheyeArea::OpticaFisheyeArea()
{
	// 默认全幅180视角, 这种鱼眼镜头可以比180度更大的视角
	constexpr float fAngleFull = static_cast<float>(M_PI);
	OpticaFisheyeArea(c_fWidthFullFrame / (2.0f * sinf(fAngleFull / 4.0f)));
}

OpticaFisheyeArea::OpticaFisheyeArea(float f135)
	: Optica(f135)
{
}

float OpticaFisheyeArea::CalcRFromTheta(float fTheta, int width)
{
	// 焦距(以像素为单位)
	float fFocus = width * m_f135 / c_fWidthFullFrame;

	// 等面积鱼眼镜头 r=2f sin（θ/2） 
	return 2.0f * fFocus * sinf(fTheta / 2.0f);
}
