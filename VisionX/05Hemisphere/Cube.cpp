#include "stdafx.h"
#include "Cube.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

Cube::Cube()
{
    init();
}

HRESULT Cube::CreateD3DResources(ComPtr<ID3D11Device>& spD3D11Dev)
{
    HRESULT hr = Geometry::CreateD3DResources(spD3D11Dev);
    if (FAILED(hr)) return hr;

    // 顶点缓冲区
    D3D11_BUFFER_DESC descVertex;
    ZeroMemory(&descVertex, sizeof(D3D11_BUFFER_DESC));
    descVertex.ByteWidth = sizeof(ColorPoint) * static_cast<UINT>(m_vVertices.size());
    descVertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA xinit;;
    ZeroMemory(&xinit, sizeof(D3D11_SUBRESOURCE_DATA));
    xinit.pSysMem = m_vVertices.data();
    xinit.SysMemPitch = 0;
    xinit.SysMemSlicePitch = 0;

    hr = spD3D11Dev->CreateBuffer(&descVertex, &xinit, &m_spVertices);
    if (FAILED(hr)) return hr;

    // 索引缓冲区
    D3D11_BUFFER_DESC descCubeIndex;
    ZeroMemory(&descCubeIndex, sizeof(D3D11_BUFFER_DESC));
    descCubeIndex.ByteWidth = static_cast<UINT>(sizeof(UINT) * m_vIndexes.size());
    descCubeIndex.BindFlags = D3D11_BIND_INDEX_BUFFER;

    xinit.pSysMem = m_vIndexes.data();

    hr = spD3D11Dev->CreateBuffer(&descCubeIndex, &xinit, &m_spIndexes);
    if (FAILED(hr)) return hr;

    return S_OK;
}

void Cube::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& spImCtx, const Space& space)
{
    // shader
    spImCtx->VSSetShader(m_spVS.Get(), nullptr, 0);
    spImCtx->PSSetShader(m_spPS.Get(), nullptr, 0);

    // constant buffer
    ConstantBuffer constBuf;
    XMMATRIX mWorld = XMMatrixTranslation(0.0f, 1.0f, 0.0f)
        * XMMatrixScaling(1.0f, 0.5f, 2.0f)
        * space.mWorld;
    constBuf.mWorldViewProjection = XMMatrixTranspose(mWorld * space.mView * space.mProjection);
    constBuf.nTextured = 0;

    spImCtx->UpdateSubresource(m_spConstBuf.Get(), 0, nullptr, &constBuf, 0, 0);
    spImCtx->VSSetConstantBuffers(0, 1, m_spConstBuf.GetAddressOf());
    spImCtx->PSSetConstantBuffers(0, 1, m_spConstBuf.GetAddressOf());

    // input layout
    spImCtx->IASetInputLayout(m_spIL.Get());
    spImCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // vertices & indexes
    const UINT stride = sizeof(ColorPoint);
    const UINT offset = 0;
    spImCtx->IASetVertexBuffers(0, 1, m_spVertices.GetAddressOf(), &stride, &offset);
    spImCtx->IASetIndexBuffer(m_spIndexes.Get(), DXGI_FORMAT_R32_UINT, 0);

    // sampler state
    spImCtx->PSSetSamplers(0, 1, m_spSamplerState.GetAddressOf());

    spImCtx->DrawIndexed(static_cast<UINT>(m_vIndexes.size()), 0, 0);
}

void Cube::init()
{
    XMFLOAT4 xmf4Color  = XMFLOAT4(0.2f, 0.6f, 0.2f, 1.0f);
    XMFLOAT4 xmf4Color2 = XMFLOAT4(0.6f, 0.0f, 0.0f, 1.0f);
    XMFLOAT4 xmf4Up(0.0f, 1.0f, 0.0f, 1.0f);
    // 一个立方体有8个顶点 POSITION, COLOR
    m_vVertices = {
        { XMFLOAT4(-1.0f,  1.0f, -1.0f, 1.0f), xmf4Color2, xmf4Up, XMFLOAT2(0.0f, 1.0f) },
        { XMFLOAT4(1.0f ,  1.0f, -1.0f, 1.0f), xmf4Color2, xmf4Up, XMFLOAT2(1.0f, 0.0f) },
        { XMFLOAT4(1.0f ,  1.0f,  1.0f, 1.0f), xmf4Color,  xmf4Up, XMFLOAT2(0.0f, 1.0f) },
        { XMFLOAT4(-1.0f,  1.0f,  1.0f, 1.0f), xmf4Color,  xmf4Up, XMFLOAT2(0.0f, 0.0f) },
        { XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), xmf4Color2, xmf4Up, XMFLOAT2(0.5f, 0.5f) },
        { XMFLOAT4(1.0f , -1.0f, -1.0f, 1.0f), xmf4Color2, xmf4Up, XMFLOAT2(0.5f, 0.5f) },
        { XMFLOAT4(1.0f , -1.0f,  1.0f, 1.0f), xmf4Color,  xmf4Up, XMFLOAT2(0.5f, 0.5f) },
        { XMFLOAT4(-1.0f, -1.0f,  1.0f, 1.0f), xmf4Color,  xmf4Up, XMFLOAT2(0.5f, 0.5f) }
    };

    // 转换为一系列的三角形,立方体有6个面,每个面拆成2个三角形,三角形3个点,共计3*2*6=36个点
    m_vIndexes = {
        3, 1, 0,
        2, 1, 3,
        0, 5, 4,
        1, 5, 0,
        3, 4, 7,
        0, 4, 3,
        1, 6, 5,
        2, 6, 1,
        2, 7, 6,
        3, 7, 2,
        6, 4, 5,
        7, 4, 6
    };
}
