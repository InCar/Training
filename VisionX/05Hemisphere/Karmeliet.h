#pragma once

// 纹理贴图
class Karmeliet
{
public:
	Karmeliet();

	// 获取图像
	const cv::Mat& GetData() const { return m_image; }

	// 载入图像
	bool LoadTexture(const std::string& strPath);
	bool LoadTextureRGBA(const cv::Mat& image);

	// 修改alpha
	void applyAlpha(float fxs = 0.0f, float fxt=1.0f);

	// 创建纹理
	HRESULT CreateTexture2D(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev, Microsoft::WRL::ComPtr<ID3D11Texture2D>& spTexture2D);
private:
	cv::Mat m_image;

	// 修正
	void calibrate(const cv::Mat& image, cv::Mat& out);
};

