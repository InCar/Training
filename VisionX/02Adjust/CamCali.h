// 镜头修正
#pragma once

class CamCali
{
public:
	CamCali();
	~CamCali();

	// 打开带有畸变的图像文件
	void OpenCameraImage(const std::string& pathJPG, std::function<void()> const& fnUpdated);

	// 获取修正后的图像
	Gdiplus::Bitmap* LockBMP();
	void UnlockBMP(Gdiplus::Bitmap* pBMP);

	// 修改镜头参数
	void UpdateCoefficients(float f, const std::vector<float>& d);
private:
	// 镜头参数K
	cv::Mat m_k;
	// 镜头参数D
	cv::Mat m_d;

	// 待修正的图像
	cv::UMat m_image;
	// 修正后的图像
	cv::UMat m_imageOutG;
	cv::Mat m_imageOut;
	// 用于显示到屏幕上
	std::unique_ptr<Gdiplus::Bitmap> m_uprtBMP;

	std::thread m_thread;
	HANDLE m_evPuls;
	std::atomic_bool m_atomQuit;
	CRITICAL_SECTION m_csKD;

	void doWork(std::function<void()> const& fnUpdated);
};


