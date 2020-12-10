#include "stdafx.h"
#include "Karmeliet.h"
#include "Beaker.h"
#include "OpticaFisheyeSin.h"

using namespace std;
using namespace DirectX;

Karmeliet::Karmeliet()
{
}

bool Karmeliet::LoadTexture(const string& strPath)
{
	// BGR
	cv::Mat image = cv::imread(strPath, cv::ImreadModes::IMREAD_COLOR);
	cv::cvtColor(image, m_image, cv::ColorConversionCodes::COLOR_BGR2RGBA);

	return true;
}

bool Karmeliet::LoadTextureRGBA(const cv::Mat& image)
{
	m_image = image;
	return true;
}

void Karmeliet::applyAlpha(float fxs, float fxt)
{
	// RGBA
	int center = m_image.cols / 2;

	for (int y = 0; y < m_image.rows; y++) {
		for (int x = 0; x < m_image.cols; x++) {
			float fDelta = fabs(1.0f * (x - center) / center);
			if (fDelta > fxs) {
				// float fAlpha = (fDelta - fxt) / (fxs - fxt);
				float fAlpha = 1.0f - ((fDelta - fxs) / (fxt - fxs)) * ((fDelta - fxs) / (fxt - fxs));

				if (fAlpha > 1.0f) fAlpha = 1.0f;
				if (fAlpha < 0.0f) fAlpha = 0.0f;

				cv::Vec4b& color = m_image.at<cv::Vec4b>(y, x);
				color[3] = static_cast<uchar>(fAlpha * 0xff);
			}
		}
	}
}

HRESULT Karmeliet::CreateTexture2D(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev, Microsoft::WRL::ComPtr<ID3D11Texture2D>& spTexture2D)
{
	D3D11_TEXTURE2D_DESC desc2D = {
		static_cast<UINT>(m_image.cols),
		static_cast<UINT>(m_image.rows),
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		{ 1, 0 },
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		0
	};

	D3D11_SUBRESOURCE_DATA xinit;
	xinit.pSysMem = m_image.data;
	xinit.SysMemPitch = 4 * m_image.cols;
	xinit.SysMemSlicePitch = 0;

	HRESULT hr = spD3D11Dev->CreateTexture2D(&desc2D, &xinit, spTexture2D.GetAddressOf());
	return hr;
}

void Karmeliet::calibrate(const cv::Mat& image, cv::Mat& out)
{
	Beaker src(image), dst;

	src.SetAzimuth(-0.03f);
	src.SetOptica(unique_ptr<OpticaFisheyeSin>(new OpticaFisheyeSin(15.0f)));
	
	dst.Load(image.cols, image.rows, cv::Vec3b(0xee, 0xee, 0xee));
	dst.SetOptica(unique_ptr<Optica>(new Optica(15.0f)));
	dst.SetAzimuth(-0.47f);

	RECT rc;
	rc.left = rc.top = 0;
	rc.right = image.cols;
	rc.bottom = image.rows;

	dst.OpticalTransfer(src, rc);

	out = dst.GetImage();
}