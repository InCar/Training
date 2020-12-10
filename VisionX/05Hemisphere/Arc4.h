#pragma once
#include "Geometry.h"

class Arc4 : public Geometry
{
public:
	Arc4();

    HRESULT CreateD3DResources(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev) override;
    void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& spImCtx, const Space& space) override;
private:
	float m_fRadius = 20.0f;
	float m_fHeight = 12.0f;

    float m_fUMax = 0.54f;

    // 左右重叠一点
    float m_fMargin = DirectX::XM_PIDIV4 / 15.0f;
    int m_nStepsArc = 9; // 圆弧分割步数

    // 底面
    std::vector<ColorPoint> m_vBottomVertices;
    std::vector<UINT> m_vBottomIndexes;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_spBottomVertices;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_spBottomIndexes;

    // 侧面
    std::vector<ColorPoint> m_vSideVertices;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_spSideVertices;

    // texture
    std::vector<Texture2DResource> m_vTex2D;

	void init();
};

