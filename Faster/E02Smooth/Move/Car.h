#pragma once
#include "phy.h"

class Car
{
public:
	Car();
	virtual ~Car();

	// 获取车辆尺寸
	FLOAT2 GetSize() { return FLOAT2(c_fWidth, c_fLength); }
	// 获取当前位置
	inline FLOAT2 GetPos();
	// 移动位置,返回新位置坐标
	FLOAT2 Move(float fDeltaX, float fDeltaY);
protected:
	// 长
	static const float c_fLength;
	// 宽
	static const float c_fWidth;
private:
	// 中心点X坐标
	float m_fX = 0.0f;
	// 中心点Y坐标
	float m_fY = 0.0f;
};

