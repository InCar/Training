#pragma once
/* Today is October 22nd, 2020,
   release date for the Mate 40 of Huawei.
   */
#include "TeaPot.h"

using namespace std;

class Mate40
{
public:
	Mate40() {};
	virtual ~Mate40() {};

	// 图像数目
	int GetCount()const { return static_cast<int>(m_vuptrTeaPots.size()); }
	// 被处理的图像
	const vector<unique_ptr<TeaPot>>& GetTeaPots() { return m_vuptrTeaPots; }
	const unique_ptr<Gdiplus::Bitmap>& GetMergedBMP() { return m_uptrXStep; }

	// 清空所有图像
	void clearAll();
	// 加载所有图像
	void LoadAll(const vector<string> & vPaths);

	// 鱼眼校正
	void CalibrateForFisheye();

	// 拼接
	void Stitching();

	// 图像配准
	void SpaceMatching();
private:
	// 每张图像对应一个TeaPot
	vector<unique_ptr<TeaPot>> m_vuptrTeaPots;

	// 镜头校正参数
	float m_fFocus = 858.92f;
	vector<float> m_fD = vector<float>{ -1.43f, 0.39f, 3.46f, 3.44f };

	// 中间结果
	cv::Mat m_xStep;
	unique_ptr<Gdiplus::Bitmap> m_uptrXStep;

	// 配准2个相邻的图像
	tuple<vector<cv::KeyPoint>, vector<cv::DMatch>, vector<cv::KeyPoint>>
		matchAdjacent(const cv::UMat& imageLeft, const cv::UMat& imageRight);

	// 求变换矩阵
	cv::Mat calcHomography(const vector<cv::DMatch>& matched, const vector<cv::KeyPoint>& keyPointLeft, const vector<cv::KeyPoint>& keyPointRight);

	// 裁剪部分关键点
	vector<cv::KeyPoint> clipKeyPoints(const vector<cv::KeyPoint>& vKeyPoints, float fClipXFrom, float fClipXTo, const cv::Size& sizeImage);
};

