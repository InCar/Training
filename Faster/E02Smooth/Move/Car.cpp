#include "stdafx.h"
#include "Car.h"

const float Car::c_fLength = 4.519f;
const float Car::c_fWidth = 1.852f;

Car::Car()
{
}

Car::~Car()
{
}

// 获取当前位置
FLOAT2 Car::GetPos()
{
	FLOAT2 pos(m_fX, m_fY);
	return pos;
}

// 移动位置
FLOAT2 Car::Move(float fDeltaX, float fDeltaY)
{
	m_fX += fDeltaX;
	m_fY += fDeltaY;

	return GetPos();
}
