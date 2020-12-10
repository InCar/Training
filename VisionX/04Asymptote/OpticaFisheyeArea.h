#pragma once
#include "Optica.h"

// 等面积鱼眼镜头 r=2f sin（θ/2） 
class OpticaFisheyeArea : public Optica
{
public:
	OpticaFisheyeArea();
	OpticaFisheyeArea(float f135);

	// 依据光学模型,从theta计算R(像素)
	float CalcRFromTheta(float fTheta, int width) override;
};

