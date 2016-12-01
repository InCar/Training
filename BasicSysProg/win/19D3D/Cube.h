#pragma once

using namespace DirectX;
using namespace Microsoft::WRL;

struct ColorPoint
{
    XMFLOAT4 Point;
    XMFLOAT4 Color;
};

class CCube
{
public:
    CCube(ComPtr<ID3D11Device> &spD3D11);
    ~CCube();

    HRESULT Init();
    ComPtr<ID3D11Buffer>& GetVertexes() { return m_spVertex; }
    ComPtr<ID3D11Buffer>& GetIndex() { return m_spCubeIndex; }
    UINT GetStride() { return sizeof(ColorPoint); }
    UINT GetVertexesCount() { return 36; }
private:
    ComPtr<ID3D11Device> &m_spD3D11;

    ColorPoint *m_pVertexes;
    UINT *m_pCubeIndex;

    ComPtr<ID3D11Buffer> m_spVertex;
    ComPtr<ID3D11Buffer> m_spCubeIndex;
};

