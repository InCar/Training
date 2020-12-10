#include "stdafx.h"
#include "CVBitmap.h"

using namespace std;

CVBitmap::CVBitmap(const cv::Mat& source)
{
	int padding = (4 - source.step1() % 4) % 4;
	if (padding == 0) {
		m_mat = source.clone();
	}
	else {
		// ²¹Æëµ½4µÄ±¶Êý
		m_mat = cv::Mat(source.rows, source.cols * 3 + padding, CV_8UC1);
		for (int i = 0; i < source.rows; i++) {
			uchar* pDst = m_mat.data + m_mat.step1() * i;
			uchar* pSrc = source.data + source.step1() * i;
			memcpy(pDst, pSrc, source.step1());
			memset(pDst + source.step1(), 0x00, padding);
		}
	}

	m_uptrBMP = unique_ptr<Gdiplus::Bitmap>(new Gdiplus::Bitmap(source.cols, source.rows, static_cast<int>(m_mat.step1()), PixelFormat24bppRGB, m_mat.data));
}
