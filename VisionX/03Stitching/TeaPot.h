#pragma once

using namespace std;

// 容纳单张图片的处理数据
class TeaPot
{
public:
	TeaPot(const string& path);
	virtual ~TeaPot();

	const unique_ptr<Gdiplus::Bitmap>& GetBMP() const { return m_uptrBmp; }
	const cv::UMat& GetImageForGPU()const { return m_imageGPU; }

	// 加载图像
	void LoadImage();
	// 鱼眼镜头修正
	void Calibrate(float focus, const vector<float>& d);
public:
	// 转换成BITMAP
	static unique_ptr<Gdiplus::Bitmap> FromOpenCVImage(const cv::Mat& image);

private:
	// 图像文件路径
	const string m_strPath;
	// 原始图像
	cv::Mat m_image;
	// GPU图像
	cv::UMat m_imageGPU;
	// 输出图像
	cv::Mat m_imageOut;
	// 输出图像BMP
	unique_ptr<Gdiplus::Bitmap> m_uptrBmp;
};

