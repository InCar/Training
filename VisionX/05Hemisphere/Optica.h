#pragma once

#include "Spatial.h"

// 标准镜头光学单元
class Optica
{
public:
	Optica();
	Optica(float f135);

	// 计算theta
	float CalcTheta(int x, int y, int width, int height);
	// 依据光学模型,从theta计算R(像素)
	virtual float CalcRFromTheta(float fTheta, int width);

	// 计算空间变换后的坐标值
	std::tuple<int,int> SpatialTransfer(int x, int y, int width, int height, const SpatialDiff& delta);

	// 设定焦距
	void SetFocus135(float f135) { m_f135 = f135; }
	// 设定鱼眼镜头参数
	virtual void SetFisheyeK(const std::vector<float>& vk) {}
protected:
	// 135全幅宽度(毫米)
	const float c_fWidthFullFrame = 36.0f;
	// 135等效焦距(毫米)
	float m_f135 = 50.0f;
};

