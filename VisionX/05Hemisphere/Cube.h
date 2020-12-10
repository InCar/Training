#pragma once

#include "geometry.h"

class Cube : public Geometry
{
public:
    Cube();

    HRESULT CreateD3DResources(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev) override;
    void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& spImCtx, const Space& space) override;
private:
    std::vector<ColorPoint> m_vVertices;
    std::vector<UINT> m_vIndexes;

    Microsoft::WRL::ComPtr<ID3D11Buffer> m_spVertices;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_spIndexes;

    // 初始化数据
    void init();
};

