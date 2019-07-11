#pragma once
#include "phy.h"

class Car
{
public:
	Car();
	virtual ~Car();

	// 获取车辆尺寸(单位:米)
	FLOAT2 GetSize() { return FLOAT2(c_fWidth, c_fLength); }
	// 获取当前位置(单位:米)
	inline FLOAT2 GetPos();
	// 移动位置,返回新位置坐标(单位:米)
	FLOAT2 Move(float fDeltaX, float fDeltaY);
protected:
	// 长(单位:米)
	static const float c_fLength;
	// 宽(单位:米)
	static const float c_fWidth;
private:
	// 中心点X坐标(单位:米)
	float m_fX = 0.0f;
	// 中心点Y坐标(单位:米)
	float m_fY = 0.0f;
};

