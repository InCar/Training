#include "stdafx.h"
#include "TeaPot.h"

TeaPot::TeaPot(const string& path)
	:m_strPath(path)
{
	OutputDebugString(L"===> TeaPot ctor\n");
}

TeaPot::~TeaPot()
{
	OutputDebugString(L"===> TeaPot destroy\n");
}

void TeaPot::LoadImage()
{
	m_image = cv::imread(m_strPath);
	m_uptrBmp = FromOpenCVImage(m_image);
	m_imageGPU = m_image.getUMat(cv::AccessFlag::ACCESS_READ);
}

void TeaPot::Calibrate(float focus, const vector<float>& d)
{
	cv::Size size = m_image.size();
	cv::Mat K = (cv::Mat_<float>(3, 3) << focus, 0.0, size.width / 2.0f, 0.0, focus, size.height / 2.0f, 0.0, 0.0, 1.0);
	cv::Mat D = (cv::Mat_<float>(1, 4) << d[0], d[1], d[2], d[3]);

	// acceleration for GPU
	cv::UMat KGPU = K.getUMat(cv::AccessFlag::ACCESS_READ);
	cv::UMat DGPU = D.getUMat(cv::AccessFlag::ACCESS_READ);
	cv::UMat imageSrcGPU = m_image.getUMat(cv::AccessFlag::ACCESS_READ);

	m_imageGPU.release();
	cv::fisheye::undistortImage(imageSrcGPU, m_imageGPU, KGPU, DGPU, KGPU, size);
	m_imageGPU.copyTo(m_imageOut);

	// m_uptrBmp = FromOpenCVImage(m_imageOut);
}

unique_ptr<Gdiplus::Bitmap> TeaPot::FromOpenCVImage(const cv::Mat& image)
{
	cv::Size size = image.size();
	return unique_ptr<Gdiplus::Bitmap>(new Gdiplus::Bitmap(size.width, size.height, static_cast<int>(image.step1()), PixelFormat24bppRGB, image.data));
}
