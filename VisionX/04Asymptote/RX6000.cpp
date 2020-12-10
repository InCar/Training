#include "stdafx.h"
#include "RX6000.h"
#include "OpticaFisheyeTanHalf.h"
#include "OpticaFisheyeArea.h"
#include "OpticaFisheyeSin.h"
#include "OpticaFisheyeTheta.h"

using namespace std;

void RX6000::LoadImages(const std::vector<std::string> vPaths)
{
	m_vuptrBeakers.clear();

	// 方位角间隔
	float fStepAzimuth = static_cast<float>(M_PI_2);
	if (vPaths.size() > 4) {
		// 不多于4个图,间隔90度分布;多于4个图,均分在圆周上
		fStepAzimuth = static_cast<float>(2.0f * M_PI / vPaths.size());
	}
	// 俯仰角
	float fPitch[] = { -0.45f, -0.3f, 0.0f, 0.0f };

	for (int i = 0; i < vPaths.size(); i++) {
		const string& path = vPaths[i];
		auto uptrBeaker = unique_ptr<Beaker>(new Beaker());
		uptrBeaker->Load(path);
		
		// auto uptrOptica = unique_ptr<Optica>(new Optica(15.0f));
		// auto uptrOptica = unique_ptr<Optica>(new OpticaFisheyeTanHalf(15.0f));
		// auto uptrOptica = unique_ptr<Optica>(new OpticaFisheyeArea(15.0f));
		auto uptrOptica = unique_ptr<Optica>(new OpticaFisheyeSin(15.0f));
		// auto uptrOptica = unique_ptr<Optica>(new OpticaFisheyeTheta(15.0f));
		uptrBeaker->SetOptica(move(uptrOptica));

		uptrBeaker->SetAzimuth(fStepAzimuth * i);

		if (i < sizeof(fPitch) / sizeof(float)) {
			uptrBeaker->SetPitch(fPitch[i]);
		}

		m_vuptrBeakers.push_back(move(uptrBeaker));
	}
}

void RX6000::SetFocus135(float f135)
{
	for (auto i = m_vuptrBeakers.begin(); i != m_vuptrBeakers.end(); i++) {
		(*i)->SetFocus135(f135);
	}
}

void RX6000::SetFisheye(const std::vector<float>& vk)
{
	for (auto i = m_vuptrBeakers.begin(); i != m_vuptrBeakers.end(); i++) {
		(*i)->SetFisheyeK(vk);
	}
}

void RX6000::SetPitch(const std::vector<float>& vPitch)
{
	for (int i = 0; i < m_vuptrBeakers.size(); i++) {
		if (i < vPitch.size()) {
			m_vuptrBeakers[i]->SetPitch(vPitch[i]);
		}
	}
}

void RX6000::SetYaw(const std::vector<float>& vYaw)
{
	for (int i = 0; i < m_vuptrBeakers.size(); i++) {
		if (i < vYaw.size()) {
			m_vuptrBeakers[i]->SetYaw(vYaw[i]);
		}
	}
}

void RX6000::Compute()
{
	if (m_vuptrBeakers.size() > 0) {
		const cv::Mat& image = m_vuptrBeakers[0]->GetImage();
		m_uptrOutputBeaker = unique_ptr<Beaker>(new Beaker());
		m_uptrOutputBeaker->Load(image.cols, image.rows, cv::Vec3b(0xee, 0xee, 0xee));
		m_uptrOutputBeaker->SetAzimuth(3.14f/4); // 北偏东
		m_uptrOutputBeaker->SetOptica(unique_ptr<Optica>(new Optica(15.0f)));

		RECT rc;
		rc.left = 0, rc.right = image.cols;
		rc.top = 0, rc.bottom = image.rows;
		// 左半幅
		RECT rcLeft = rc;
		rcLeft.right = rc.right / 2;
		m_uptrOutputBeaker->OpticalTransfer(*m_vuptrBeakers[0].get(), rcLeft);
		// return;
	    // 右半幅
		RECT rcRight = rc;
		rcRight.left = rc.right / 2;
		if(m_vuptrBeakers.size() > 1)
			m_uptrOutputBeaker->OpticalTransfer(*m_vuptrBeakers[1].get(), rcRight);
	}
	else {
		m_uptrOutputBeaker.reset();
	}
}