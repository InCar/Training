#pragma once
#include "Optica.h"

/* 带修正的等角鱼眼镜头
*  r = f*θ(k0+k1*θ^2+k2*θ^4+k3*θ^6+k4*θ^8)
*  当k0=1时,和opencv使用的鱼眼模型一致
*/
class OpticaFisheyeTheta : public Optica
{
public:
	OpticaFisheyeTheta();
	OpticaFisheyeTheta(float f135);

	// 设定系数值
	void SetK(const std::vector<float>& vk) { m_vk = vk; }

	// 依据光学模型,从theta计算R(像素)
	float CalcRFromTheta(float fTheta, int width) override;
	// 设定参数
	void SetFisheyeK(const std::vector<float>& vk) override;
private:
	std::vector<float> m_vk = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
};

