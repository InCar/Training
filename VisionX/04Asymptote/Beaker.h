#pragma once
#include "Optica.h"
#include "Spatial.h"

// 包含从一个特定坐标位置用一个特定镜头拍摄的平面图像
class Beaker
{
public:
	Beaker();

	const std::string& GetPath() const { return m_strPath; }
	const cv::Mat& GetImage()const { return m_image; };
	
public:
	// 用color指定的颜色初始化图像
	void Load(int width, int height, const cv::Vec3b& color);
	// 使用指定的文件作为图像
	void Load(const std::string& path);

	// 查找指定位置的像素颜色值
	std::unique_ptr<cv::Vec3b> LookupPixel(float fTheta, int x, int y) const;

	// 设定方位角(弧度,N=0 E=Pi/2)
	void SetAzimuth(float fAzimuth) { m_spatial.SetAzimuth(fAzimuth); }
	void SetYaw(float fYaw) { m_spatial.SetYaw(fYaw); }
	// 设定俯仰角(弧度,上正下负)
	void SetPitch(float fPitch) { m_spatial.SetPitch(fPitch); }
	float GetPitch() const { return m_spatial.GetPitch(); }
	// 设定光学镜头
	void SetOptica(std::unique_ptr<Optica>&& uptrOptica);
	// 设定焦距
	void SetFocus135(float f135);
	// 设定鱼眼镜头参数
	void SetFisheyeK(const std::vector<float>& vk);

public:
	// 像素直拷
	void CopyImage(const Beaker& src);
	// 光学传送
	void OpticalTransfer(const Beaker& src, const RECT& rc);
	// 把本地坐值转换为源坐标值
	std::tuple<int, int> SpatialTransfer(int x, int y, const Beaker& src);
private:
	// 空间关系
	Spatial m_spatial;
	// 光学镜头
	std::unique_ptr<Optica> m_uptrOptica;

	std::string m_strPath; // 图像路径
	cv::Mat m_image; // 图像

	// 梯形校正
	void trapezoidAdjust(int&x, int y, int width, int height, float fPitch);
};

