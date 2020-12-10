#include "stdafx.h"
#include "OpticaFisheyeTheta.h"

using namespace std;

OpticaFisheyeTheta::OpticaFisheyeTheta()
{
	// 默认全幅180视角, 这种鱼眼镜头可以比180度更大的视角
	constexpr float fAngleFull = static_cast<float>(M_PI);
	OpticaFisheyeTheta(c_fWidthFullFrame / fAngleFull);
}

OpticaFisheyeTheta::OpticaFisheyeTheta(float f135)
	: Optica(f135)
{
}

float OpticaFisheyeTheta::CalcRFromTheta(float fTheta, int width)
{
	// 焦距(以像素为单位)
	float fFocus = width * m_f135 / c_fWidthFullFrame;

	// 等角鱼眼镜头 r = f*θ(k0+k1*θ^2+k2*θ^4+k3*θ^6+k4*θ^8)
	float fSum = 0.0f;
	for (int i = 0; i < m_vk.size(); i++) {
		fSum += m_vk[i] * static_cast<float>(pow(fTheta, 2 * i));
	}
	return fFocus * fTheta * fSum;
}

void OpticaFisheyeTheta::SetFisheyeK(const std::vector<float>& vk)
{
	m_vk = vk;
}
