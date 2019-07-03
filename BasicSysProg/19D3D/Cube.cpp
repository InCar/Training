#include "StdAfx.h"
#include "Cube.h"

using namespace DirectX;

CCube::CCube(ComPtr<ID3D11Device> &spD3D11)
    :m_spD3D11(spD3D11)
{
    // 一个立方体有8个顶点
    m_pVertexes = new ColorPoint[8];
    m_pVertexes[0].Point = XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f);
    m_pVertexes[0].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    m_pVertexes[1].Point = XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f);
    m_pVertexes[1].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    m_pVertexes[2].Point = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    m_pVertexes[2].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    m_pVertexes[3].Point = XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f);
    m_pVertexes[3].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    m_pVertexes[4].Point = XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f);
    m_pVertexes[4].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    m_pVertexes[5].Point = XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f);
    m_pVertexes[5].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    m_pVertexes[6].Point = XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f);
    m_pVertexes[6].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    m_pVertexes[7].Point = XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f);
    m_pVertexes[7].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    // 转换为一系列的三角形,立方体有6个面,每个面拆成2个三角形,三角形3个点,共计3*2*6=36个点
    m_pCubeIndex = new UINT[36];
    m_pCubeIndex[0] = 3, m_pCubeIndex[1] = 1, m_pCubeIndex[2] = 0;
    m_pCubeIndex[3] = 2, m_pCubeIndex[4] = 1, m_pCubeIndex[5] = 3;
    m_pCubeIndex[6] = 0, m_pCubeIndex[7] = 5, m_pCubeIndex[8] = 4;
    m_pCubeIndex[9] = 1, m_pCubeIndex[10] = 5, m_pCubeIndex[11] = 0;
    m_pCubeIndex[12] = 3, m_pCubeIndex[13] = 4, m_pCubeIndex[14] = 7;
    m_pCubeIndex[15] = 0, m_pCubeIndex[16] = 4, m_pCubeIndex[17] = 3;
    m_pCubeIndex[18] = 1, m_pCubeIndex[19] = 6, m_pCubeIndex[20] = 5;
    m_pCubeIndex[21] = 2, m_pCubeIndex[22] = 6, m_pCubeIndex[23] = 1;
    m_pCubeIndex[24] = 2, m_pCubeIndex[25] = 7, m_pCubeIndex[26] = 6;
    m_pCubeIndex[27] = 3, m_pCubeIndex[28] = 7, m_pCubeIndex[29] = 2;
    m_pCubeIndex[30] = 6, m_pCubeIndex[31] = 4, m_pCubeIndex[32] = 5;
    m_pCubeIndex[33] = 7, m_pCubeIndex[34] = 4, m_pCubeIndex[35] = 6;
}


CCube::~CCube()
{
    delete[] m_pVertexes;
    delete[] m_pCubeIndex;
}

HRESULT CCube::Init()
{
    // 顶点缓冲区
    D3D11_BUFFER_DESC descVertex;
    ZeroMemory(&descVertex, sizeof(D3D11_BUFFER_DESC));
    descVertex.ByteWidth = sizeof(ColorPoint) * 8;
    descVertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA xinit;;
    ZeroMemory(&xinit, sizeof(D3D11_SUBRESOURCE_DATA));
    xinit.pSysMem = m_pVertexes;
    xinit.SysMemPitch = 0;
    xinit.SysMemSlicePitch = 0;

    HRESULT hr = m_spD3D11->CreateBuffer(&descVertex, &xinit, &m_spVertex);
    if (FAILED(hr)) return hr;

    // 索引缓冲区
    D3D11_BUFFER_DESC descCubeIndex;
    ZeroMemory(&descCubeIndex, sizeof(D3D11_BUFFER_DESC));
    descCubeIndex.ByteWidth = sizeof(UINT) * 36;
    descCubeIndex.BindFlags = D3D11_BIND_INDEX_BUFFER;

    xinit.pSysMem = m_pCubeIndex;

    hr = m_spD3D11->CreateBuffer(&descCubeIndex, &xinit, &m_spCubeIndex);
    if (FAILED(hr)) return hr;

    return S_OK;
}
