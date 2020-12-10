#include "stdafx.h"
#include "Beaker.h"

using namespace std;

Beaker::Beaker()
{

}

Beaker::Beaker(const cv::Mat& image)
{
	m_image = image;
}

void Beaker::CopyImage(const Beaker& src)
{
	const cv::Mat& source = src.GetImage();

	for (int i = 0; i < m_image.rows; i++)
	{
		for (int j = 0; j < m_image.cols; j++)
		{
			uchar* pDst = m_image.data + m_image.step1() * i + 3 * j;
			uchar* pSrc = source.data + source.step1() * i + 3 * j;
			if (i < source.rows && j < source.cols)
			{
				memcpy(pDst, pSrc, 3);
			}
			else {
				// memset(pDst, 0x00, 3);
			}
		}
	}
}

void Beaker::OpticalTransfer(const Beaker& src, const RECT& rc)
{
	// 逐点计算
	for (int y = 0; y < m_image.rows; y++)
	{
		for (int x = 0; x < m_image.cols; x++)
		{

			if (x >= rc.left && x < rc.right && y >= rc.top && y < rc.bottom)
			{
				int x2, y2;
				tie(x2, y2) = SpatialTransfer(x, y, src);
				
				// 梯形校正 
				//trapezoidAdjust(x2, y2, m_image.cols, m_image.rows, src.GetPitch());

				float fTheta = m_uptrOptica->CalcTheta(x2, y2, m_image.cols, m_image.rows);

				// 根据fTheta,x,y查找对应的像素
				auto uptrColor = src.LookupPixel(fTheta, x2, y2);

				// 找不到的保留原来的色彩
				if (uptrColor)
					m_image.at<cv::Vec3b>(y, x) = *uptrColor;
			}
		}
	}
}

std::tuple<int, int> Beaker::SpatialTransfer(int x, int y, const Beaker& src)
{	
	auto delta = m_spatial - src.m_spatial;
	return m_uptrOptica->SpatialTransfer(x, y, m_image.cols, m_image.rows, delta);
}

void Beaker::trapezoidAdjust(int& x, int y, int width, int height, float fPitch)
{
	// 梯形修正量和俯仰角相关
	float fk = 1.0f + 3.0f * tanf(-fPitch);

	// 中心点(以像素为单位)
	const float fX0 = m_image.cols / 2.0f;
	const float fY0 = m_image.rows / 2.0f;
	float fU = x - fX0;
	float fV = y - fY0;
	float fU2 = fU * ((1.0f - fk) * fV / height);

	x = static_cast<int>(fX0 + fU + fU2);
}

void Beaker::Load(int width, int height, const cv::Vec3b& color)
{
	m_image = cv::Mat(height, width, CV_8UC3, color);
	m_strPath.clear();
}

void Beaker::Load(const std::string& path)
{
	m_image = cv::imread(path);
	m_strPath = path;
}

unique_ptr<cv::Vec3b> Beaker::LookupPixel(float fTheta, int x, int y) const
{
	// 中心点(以像素为单位)
	const float fX0 = m_image.cols / 2.0f;
	const float fY0 = m_image.rows / 2.0f;

	float fR = m_uptrOptica->CalcRFromTheta(fTheta, m_image.cols);
	float fAngle = atan2f(y - fY0, x - fX0);
	float fU = fR * cosf(fAngle);
	float fV = fR * sinf(fAngle);

	// 这里取最近的一个点,更好的算法是采样平均
	int nU = static_cast<int>(roundf(fX0 + fU));
	int nV = static_cast<int>(roundf(fY0 + fV));

	// 如果找不到对应的点则返回nullptr
	if (nU < 0 || nU >= m_image.cols || nV < 0 || nV >= m_image.rows)
		return nullptr;
	else {
		cv::Vec3b color = m_image.at<cv::Vec3b>(nV, nU);
		return unique_ptr<cv::Vec3b>(new cv::Vec3b(color));
	}
}

void Beaker::SetOptica(unique_ptr<Optica>&& uptrOptica)
{
	m_uptrOptica = move(uptrOptica);
}

void Beaker::SetFocus135(float f135)
{
	m_uptrOptica->SetFocus135(f135);
}

void Beaker::SetFisheyeK(const std::vector<float>& vk)
{
	m_uptrOptica->SetFisheyeK(vk);
}
