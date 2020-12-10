#include "stdafx.h"
#include "CamCali.h"

CamCali::CamCali()
{
	InitializeCriticalSection(&m_csKD);
	m_evPuls = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

CamCali::~CamCali()
{
	if (m_thread.joinable()) {
		m_atomQuit = true;
		SetEvent(m_evPuls);
		m_thread.join();
	}

	DeleteCriticalSection(&m_csKD);
	CloseHandle(m_evPuls);
}

void CamCali::OpenCameraImage(const std::string& pathJPG, std::function<void()> const& fnUpdated)
{
	m_image = cv::imread(pathJPG).getUMat(cv::AccessFlag::ACCESS_READ);
	m_thread = std::thread(&CamCali::doWork, this, fnUpdated);
}

Gdiplus::Bitmap* CamCali::LockBMP()
{
	EnterCriticalSection(&m_csKD);
	return m_uprtBMP.get();
}

void CamCali::UnlockBMP(Gdiplus::Bitmap* pBMP)
{
	LeaveCriticalSection(&m_csKD);
}


void CamCali::UpdateCoefficients(float f, const std::vector<float>& d)
{
	EnterCriticalSection(&m_csKD);
	cv::Size size = m_image.size();
	m_k = (cv::Mat_<float>(3, 3) << f, 0.0, size.width / 2.0f, 0.0, f, size.height / 2.0f, 0.0, 0.0, 1.0);
    m_d = (cv::Mat_<double>(1, 4) << d[0], d[1], d[2], d[3]);
	LeaveCriticalSection(&m_csKD);

	SetEvent(m_evPuls);
}

void CamCali::doWork(std::function<void()> const& fnUpdated)
{
	while (true) {
		WaitForSingleObject(m_evPuls, INFINITE);
		if (m_atomQuit) break;

		EnterCriticalSection(&m_csKD);
		cv::UMat k = m_k.getUMat(cv::AccessFlag::ACCESS_READ);
		cv::UMat d = m_d.getUMat(cv::AccessFlag::ACCESS_READ);
		LeaveCriticalSection(&m_csKD);

		cv::Size size = m_image.size();
		cv::fisheye::undistortImage(m_image, m_imageOutG, k, d, k, size);

		EnterCriticalSection(&m_csKD);
		m_imageOutG.copyTo(m_imageOut);
		m_uprtBMP.reset();
		m_uprtBMP = std::unique_ptr<Gdiplus::Bitmap>(new Gdiplus::Bitmap(size.width, size.height, static_cast<int>(m_imageOut.step1()), PixelFormat24bppRGB, m_imageOut.data));
		LeaveCriticalSection(&m_csKD);

		fnUpdated();
	}
}
