#pragma once

#include "geometry.h"

class Cylinder : public Geometry
{
public:
    Cylinder();

    HRESULT CreateD3DResources(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev) override;
    void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& spImCtx, const Space& space) override;
private:
    float m_fRadius = 20.0f;
    float m_fHeight = 12.0f;

    int m_nStepsArc = 36; // 圆弧分割步数
    // 底面
    std::vector<ColorPoint> m_vBottomVertices;
    std::vector<UINT> m_vBottomIndexes;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_spBottomVertices;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_spBottomIndexes;

    // 侧面
    std::vector<ColorPoint> m_vSideVertices;
    std::vector<UINT> m_vSideIndexes;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_spSideVertices;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_spSideIndexes;

    // texture
    std::vector<Texture2DResource> m_vTex2D;

    // 初始化数据
    void init();
};
