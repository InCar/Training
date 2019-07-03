#include "stdafx.h"
#include "Cube2.h"


CCube2::CCube2(ComPtr<ID3D11Device> &spD3D11)
    :m_spD3D11(spD3D11)
{
    // 一个立方体有6个面,每个面4个顶点,共计24个
    m_pVertexes = new ColorPoint2[24];
    m_pVertexes[0].Point = XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f);    m_pVertexes[0].Normal = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    m_pVertexes[1].Point = XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f);     m_pVertexes[1].Normal = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    m_pVertexes[2].Point = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);      m_pVertexes[2].Normal = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    m_pVertexes[3].Point = XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f);     m_pVertexes[3].Normal = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    m_pVertexes[4].Point = XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f);   m_pVertexes[4].Normal = XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f);
    m_pVertexes[5].Point = XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f);    m_pVertexes[5].Normal = XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f);
    m_pVertexes[6].Point = XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f);     m_pVertexes[6].Normal = XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f);
    m_pVertexes[7].Point = XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f);    m_pVertexes[7].Normal = XMFLOAT4(0.0f, -1.0f, 0.0f, 1.0f);

    m_pVertexes[8].Point = XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f);    m_pVertexes[8].Normal = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
    m_pVertexes[9].Point = XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f);   m_pVertexes[9].Normal = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
    m_pVertexes[10].Point = XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f);   m_pVertexes[10].Normal = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
    m_pVertexes[11].Point = XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f);    m_pVertexes[11].Normal = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);

    m_pVertexes[12].Point = XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f);    m_pVertexes[12].Normal = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
    m_pVertexes[13].Point = XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f);   m_pVertexes[13].Normal = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
    m_pVertexes[14].Point = XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f);    m_pVertexes[14].Normal = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
    m_pVertexes[15].Point = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);     m_pVertexes[15].Normal = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

    m_pVertexes[16].Point = XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f);  m_pVertexes[16].Normal = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
    m_pVertexes[17].Point = XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f);   m_pVertexes[17].Normal = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
    m_pVertexes[18].Point = XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f);    m_pVertexes[18].Normal = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
    m_pVertexes[19].Point = XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f);   m_pVertexes[19].Normal = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);

    m_pVertexes[20].Point = XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f);   m_pVertexes[20].Normal = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
    m_pVertexes[21].Point = XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f);    m_pVertexes[21].Normal = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
    m_pVertexes[22].Point = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);     m_pVertexes[22].Normal = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
    m_pVertexes[23].Point = XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f);    m_pVertexes[23].Normal = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

    // 转换为一系列的三角形,立方体有6个面,每个面拆成2个三角形,三角形3个点,共计3*2*6=36个点
    m_pCubeIndex = new UINT[36];
    m_pCubeIndex[0] = 3, m_pCubeIndex[1] = 1, m_pCubeIndex[2] = 0;
    m_pCubeIndex[3] = 2, m_pCubeIndex[4] = 1, m_pCubeIndex[5] = 3;

    m_pCubeIndex[6] = 6, m_pCubeIndex[7] = 4, m_pCubeIndex[8] = 5;
    m_pCubeIndex[9] = 7, m_pCubeIndex[10] = 4, m_pCubeIndex[11] = 6;

    m_pCubeIndex[12] = 11, m_pCubeIndex[13] = 9, m_pCubeIndex[14] = 8;
    m_pCubeIndex[15] = 10, m_pCubeIndex[16] = 9, m_pCubeIndex[17] = 11;

    m_pCubeIndex[18] = 14, m_pCubeIndex[19] = 12, m_pCubeIndex[20] = 13;
    m_pCubeIndex[21] = 15, m_pCubeIndex[22] = 12, m_pCubeIndex[23] = 14;

    m_pCubeIndex[24] = 19, m_pCubeIndex[25] = 17, m_pCubeIndex[26] = 16;
    m_pCubeIndex[27] = 18, m_pCubeIndex[28] = 17, m_pCubeIndex[29] = 19;

    m_pCubeIndex[30] = 22, m_pCubeIndex[31] = 20, m_pCubeIndex[32] = 21;
    m_pCubeIndex[33] = 23, m_pCubeIndex[34] = 20, m_pCubeIndex[35] = 22;
}


CCube2::~CCube2()
{
    delete[] m_pVertexes;
    delete[] m_pCubeIndex;
}

HRESULT CCube2::Init()
{
    // 顶点缓冲区
    D3D11_BUFFER_DESC descVertex;
    ZeroMemory(&descVertex, sizeof(D3D11_BUFFER_DESC));
    descVertex.ByteWidth = sizeof(ColorPoint2) * 24;
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

