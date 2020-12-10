#include "stdafx.h"
#include "OpticaFisheyeTanHalf.h"

using namespace std;

OpticaFisheyeTanHalf::OpticaFisheyeTanHalf()
{
	// 默认全幅180视角, 这种鱼眼镜头可以比180度更大的视角
	constexpr float fAngleFull = static_cast<float>(M_PI);
	OpticaFisheyeTanHalf(c_fWidthFullFrame / (2.0f * tanf(fAngleFull / 4.0f)));
}

OpticaFisheyeTanHalf::OpticaFisheyeTanHalf(float f135)
	: Optica(f135)
{
}

float OpticaFisheyeTanHalf::CalcRFromTheta(float fTheta, int width)
{
	// 焦距(以像素为单位)
	float fFocus = width * m_f135 / c_fWidthFullFrame;

	// 赤平鱼眼镜头 r=2f tan（θ/2） 
	return 2.0f * fFocus * tanf(fTheta / 2.0f);
}