#pragma once

// 空间相对关系
class SpatialDiff
{
public:
	float fDeltaAzimuth = 0.0f; // 水平
	float fDeltaPitch = 0.0f; // 垂直
};

// 空间关系
class Spatial
{
public:
	Spatial();
	Spatial(float fAzimuth) { m_fAzimuth = fAzimuth; };

	void SetAzimuth(float fAzimuth) { m_fAzimuth = fAzimuth; }
	void SetYaw(float fYaw) { m_fYaw = fYaw; }
	void SetPitch(float fPitch) { m_fPitch = fPitch; }
	float GetPitch() const { return m_fPitch; }

	SpatialDiff operator-(const Spatial& right);

private:
	// 初始方位角(弧度, N=0 E=Pi/2 S=Pi W=3Pi/2)
	float m_fAzimuth = 0.0f;
	// 水平偏向角(弧度 右正左负)
	float m_fYaw = 0.0f;
	// 俯仰偏向角(弧度 上正下负)
	float m_fPitch = 0.0f;
	// 旋转(弧度)
	float m_fRotate = 0.0f;
	// TODO: 空间位置(x,y,z)
	
};

