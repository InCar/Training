#pragma once
#include "Optica.h"

// 赤平鱼眼镜头
class OpticaFisheyeTanHalf : public Optica
{
public:
	OpticaFisheyeTanHalf();
	OpticaFisheyeTanHalf(float f135);

	// 依据光学模型,从theta计算R(像素)
	float CalcRFromTheta(float fTheta, int width) override;
};

