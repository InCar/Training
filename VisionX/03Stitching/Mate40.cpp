#include "stdafx.h"
#include "Mate40.h"


void Mate40::clearAll()
{
	m_vuptrTeaPots.clear();

}

void Mate40::LoadAll(const vector<string>& vPaths)
{
	// 清空
	m_vuptrTeaPots.clear();

	for (const string& s : vPaths) {
		m_vuptrTeaPots.push_back(unique_ptr<TeaPot>(new TeaPot(s)));
	}

	// 逐个加载
	for (const auto& uptrTeaPot : m_vuptrTeaPots) {
		uptrTeaPot->LoadImage();
	}
}

void Mate40::CalibrateForFisheye()
{
	// 鱼眼校正
	for (const auto& uptrTeaPot : m_vuptrTeaPots) {
		uptrTeaPot->Calibrate(m_fFocus, m_fD);
	}
}

void Mate40::Stitching()
{
	cv::Ptr<cv::Stitcher> cvptrStitcher = cv::Stitcher::create(cv::Stitcher::Mode::SCANS);

	vector<cv::UMat> vSource;
	for (const auto& uptrTeaPot : m_vuptrTeaPots) {
		vSource.push_back(uptrTeaPot->GetImageForGPU());
	}

	cvptrStitcher->setPanoConfidenceThresh(0.1);
	cv::Stitcher::Status status = cvptrStitcher->stitch(vSource, m_xStep);
	m_uptrXStep = TeaPot::FromOpenCVImage(m_xStep);
}

void Mate40::SpaceMatching()
{
	// 相邻图像配准
	for (int i = 1; i < m_vuptrTeaPots.size(); i++) {
		if (i + 1 <= m_vuptrTeaPots.size()) {
			// i-1 & i
			const auto& uptrLeft = m_vuptrTeaPots[i - 1];
			const auto& uptrRight = m_vuptrTeaPots[i];

			vector<cv::KeyPoint> keyPointLeft, keyPointRight;
			vector<cv::DMatch> matched;
			// C++ 17 not supported yet
			// [keyPointLeft, matched, keyPointRight] = matchAdjacent(uptrLeft->GetImageForGPU(), uptrRight->GetImageForGPU());
			tie(keyPointLeft, matched, keyPointRight) = matchAdjacent(uptrLeft->GetImageForGPU(), uptrRight->GetImageForGPU());
			cv::Mat homo3x3 = calcHomography(matched, keyPointLeft, keyPointRight);

			// 先只处理2个图
			break;
		}

		if (i == 3 && m_vuptrTeaPots.size() == 4) {
			// TODO: 第4张和第1拼环
			// i & 1st 
			// const auto& uptrLeft = m_vuptrTeaPots[i];
			// const auto& uptrRight = m_vuptrTeaPots[0];
			// matchAdjacent(uptrLeft->GetImageForGPU(), uptrRight->GetImageForGPU());
		}
	}
}

tuple<vector<cv::KeyPoint>, vector<cv::DMatch>, vector<cv::KeyPoint>>
	Mate40::matchAdjacent(const cv::UMat& imageLeft, const cv::UMat& imageRight)
{
	// 转换为灰度
	// cv::UMat imageL, imageR;
	// cv::cvtColor(imageLeft, imageL, cv::COLOR_RGB2GRAY);
	// cv::cvtColor(imageRight, imageR, cv::COLOR_RGB2GRAY);

	// 算法
	// cv::Ptr<cv::ORB> cvptrX = cv::ORB::create();
	// cv::Ptr<cv::BRISK> cvptrX = cv::BRISK::create(30, 5, 4.0f);
	// cv::Ptr<cv::SIFT> cvptrX = cv::SIFT::create();
	cv::Ptr<cv::AKAZE> cvptrX = cv::AKAZE::create(cv::AKAZE::DESCRIPTOR_MLDB, 0, 3, 0.002f, 4, 4, cv::KAZE::DIFF_PM_G2);

	vector<cv::KeyPoint> keyPointLeft, keyPointRight;
	cvptrX->detect(imageLeft, keyPointLeft);
	cvptrX->detect(imageRight, keyPointRight);

	// 已知 左图的右半部 和 右图的左半部 存在对应关系, 所以可以裁剪掉一些点
	cv::Size sizeLeft = imageLeft.size(), sizeRight = imageRight.size();
	keyPointLeft = clipKeyPoints(keyPointLeft, 0.5f, 1.0f, sizeLeft);
	keyPointRight = clipKeyPoints(keyPointRight, 0.0f, 0.5f, sizeRight);

	cv::UMat descLeft, descRight;
	cvptrX->compute(imageLeft, keyPointLeft, descLeft);
	cvptrX->compute(imageRight, keyPointRight, descRight);

	cv::BFMatcher matcher(cv::NORM_HAMMING, true);
	// cv::FlannBasedMatcher matcher; // for SIFT
	vector<cv::DMatch> matchedPoints, matchedPointsBetter;
	matcher.match(descLeft, descRight, matchedPoints);

	// 选出比较好的匹配点
	// Y相差不应大太,设定在20%以内
	int maxDeltaY = static_cast<int>(imageLeft.size().height * 0.2f);
	for (const auto& matched : matchedPoints) {
		const auto& ptLeft = keyPointLeft[matched.queryIdx];
		const auto& ptRight = keyPointRight[matched.trainIdx];
		if (abs(ptLeft.pt.y - ptRight.pt.y) < maxDeltaY)
			matchedPointsBetter.push_back(matched);
	}

	wchar_t buf[1024];
	swprintf_s(buf, L"=== matched ===> %llu\n", matchedPoints.size());
	OutputDebugString(buf);

	for (const auto& point : matchedPoints) {
		const auto& ptLeft = keyPointLeft[point.queryIdx];
		const auto& ptRight = keyPointRight[point.trainIdx];
		swprintf_s(buf, L"===> %5d -> %5d : %6.2f | %5d\n", point.queryIdx, point.trainIdx, point.distance, static_cast<int>(ptLeft.pt.y-ptRight.pt.y));
		OutputDebugString(buf);
	}

	cv::drawMatches(imageLeft, keyPointLeft, imageRight, keyPointRight, matchedPointsBetter, m_xStep);
	m_uptrXStep = TeaPot::FromOpenCVImage(m_xStep);

	return { keyPointLeft, matchedPointsBetter, keyPointRight };
}

cv::Mat Mate40::calcHomography(const vector<cv::DMatch>& matched, const vector<cv::KeyPoint>& keyPointLeft, const vector<cv::KeyPoint>& keyPointRight)
{
	vector<cv::Point2f> vLeftPoints, vRightPoints;
	for (const auto& m : matched) {
		vLeftPoints.push_back(keyPointLeft[m.queryIdx].pt);
		vRightPoints.push_back(keyPointRight[m.trainIdx].pt);
	}

	// 左图不变,求右图变换矩阵
	cv::Mat matHomo3x3 = cv::findHomography(vRightPoints, vLeftPoints, cv::RANSAC);
	stringstream ss = (stringstream() << matHomo3x3 << "\n");
	OutputDebugStringA(ss.str().c_str());

	return matHomo3x3;
}

vector<cv::KeyPoint> Mate40::clipKeyPoints(const vector<cv::KeyPoint>& vKeyPoints, float fClipXFrom, float fClipXTo, const cv::Size& sizeImage)
{
	float fMin = sizeImage.width * fClipXFrom;
	float fMax = sizeImage.width * fClipXTo;

	vector<cv::KeyPoint> vKeep;
	for (const auto& point : vKeyPoints) {
		if (point.pt.x >= fMin && point.pt.x <= fMax) {
			vKeep.push_back(point);
		}
	}

	return vKeep;
}
