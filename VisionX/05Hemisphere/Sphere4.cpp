#include "stdafx.h"
#include "Sphere4.h"
#include "Karmeliet.h"
#include <direct.h>

using namespace std;
using namespace Microsoft::WRL;
using namespace DirectX;

Sphere4::Sphere4()
{
	init();
}

HRESULT Sphere4::CreateD3DResources(ComPtr<ID3D11Device>& spD3D11Dev)
{
    HRESULT hr = Geometry::CreateD3DResources(spD3D11Dev);
    if (FAILED(hr)) return hr;

    auto fnCreateD3DBuf = [&spD3D11Dev](const vector<ColorPoint>& vVertices, const vector<UINT>& vIndexes,
        ComPtr<ID3D11Buffer>& spVertices, ComPtr<ID3D11Buffer>& spIndexes)->HRESULT
    {
        // 顶点缓冲区
        D3D11_BUFFER_DESC descVertex;
        ZeroMemory(&descVertex, sizeof(D3D11_BUFFER_DESC));
        descVertex.ByteWidth = sizeof(ColorPoint) * static_cast<UINT>(vVertices.size());
        descVertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA xinit;;
        ZeroMemory(&xinit, sizeof(D3D11_SUBRESOURCE_DATA));
        xinit.pSysMem = vVertices.data();
        xinit.SysMemPitch = 0;
        xinit.SysMemSlicePitch = 0;

        HRESULT hr = spD3D11Dev->CreateBuffer(&descVertex, &xinit, &spVertices);
        if (FAILED(hr)) return hr;

        // 索引缓冲区
        if (vIndexes.size() > 0) {
            D3D11_BUFFER_DESC descCubeIndex;
            ZeroMemory(&descCubeIndex, sizeof(D3D11_BUFFER_DESC));
            descCubeIndex.ByteWidth = static_cast<UINT>(sizeof(UINT) * vIndexes.size());
            descCubeIndex.BindFlags = D3D11_BIND_INDEX_BUFFER;

            xinit.pSysMem = vIndexes.data();

            hr = spD3D11Dev->CreateBuffer(&descCubeIndex, &xinit, &spIndexes);
            if (FAILED(hr)) return hr;
        }

        return S_OK;
    };

    hr = fnCreateD3DBuf(m_vSideVertices, m_vSideIndexes, m_spSideVertices, m_spSideIndexes);
    if (FAILED(hr)) return hr;

    // texture
    char buf[_MAX_PATH];
    string strPath(_getcwd(buf, _MAX_PATH));
    vector<string> vPaths = {
        strPath + R"(/carset2/set2/1front.jpg)", // Pi*0/2
        strPath + R"(/carset2/set2/2right.jpg)", // Pi*1/2
        strPath + R"(/carset2/set2/3back.jpg)",  // Pi*2/2
        strPath + R"(/carset2/set2/4left.jpg)"   // Pi*3/2
    };
    for (const string& strPath : vPaths) {
        Karmeliet kaImage;
        kaImage.LoadTexture(strPath);
        // kaImage.applyAlpha(0.3f, 1.0f);
        kaImage.applyAlpha(0.3f, 0.6f);

        Texture2DResource tex2DRes;
        hr = loadTexture2D(spD3D11Dev, kaImage.GetData(), tex2DRes.spD3D11Tex2D, tex2DRes.spD3D11SRV);
        if (FAILED(hr)) return hr;

        m_vTex2D.push_back(tex2DRes);
    }
    // 位置
    m_vCamPos = {
        {   0.0f,   0.0f,   1.7f }, // front
        {   1.0f,   0.5f,   0.0f }, // right
        {   0.0f,   0.0f,  -3.0f }, // back
        {  -1.0f,   0.5f,   0.0f }  // left
    };

    // 下倾角
    m_vPitch = { 10.0f, 20.0f, 21.0f, 22.0f };
    for (float& fPitch : m_vPitch) {
        fPitch = XM_PI * fPitch / 180.0f; // 弧度
    }

    // depth state
    D3D11_DEPTH_STENCIL_DESC descDS = { 0 };
    descDS.DepthEnable = FALSE;
    descDS.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    descDS.DepthFunc = D3D11_COMPARISON_LESS;
    descDS.StencilEnable = FALSE;
    spD3D11Dev->CreateDepthStencilState(&descDS, m_spDepthStencilState.GetAddressOf());

    return S_OK;
}

void Sphere4::Draw(ComPtr<ID3D11DeviceContext>& spImCtx, const Space& space)
{
    // shader
    spImCtx->VSSetShader(m_spVS.Get(), NULL, 0);
    spImCtx->PSSetShader(m_spPS.Get(), NULL, 0);

    // constant buffer
    ConstantBuffer constBuf;
    spImCtx->VSSetConstantBuffers(0, 1, m_spConstBuf.GetAddressOf());
    spImCtx->PSSetConstantBuffers(0, 1, m_spConstBuf.GetAddressOf());

    // input layout
    spImCtx->IASetInputLayout(m_spIL.Get());
    // sampler state
    spImCtx->PSSetSamplers(0, 1, m_spSamplerState.GetAddressOf());
    // depth state
    UINT nStencilRef;
    ComPtr<ID3D11DepthStencilState> spStateDS;
    spImCtx->OMGetDepthStencilState(spStateDS.GetAddressOf(), &nStencilRef);
    spImCtx->OMSetDepthStencilState(m_spDepthStencilState.Get(), 0);

    const UINT stride = sizeof(ColorPoint);
    const UINT offset = 0;

    vector<int> vSequence = { 0, 2, 1, 3 };
    // vSequence = { 0, 2, 3 };
    for (int i = 0; i < vSequence.size(); i++) {
        int x = vSequence[i];
        XMMATRIX mRotate = XMMatrixRotationX(m_vPitch[x]) * XMMatrixRotationY(XM_PIDIV2 * x);
        XMMATRIX mMove = XMMatrixTranslation(m_vCamPos[x].x, m_vCamPos[x].y, m_vCamPos[x].z);
        constBuf.mWorldViewProjection = XMMatrixTranspose(mRotate * mMove * space.mWorld * space.mView * space.mProjection);

        // side
        constBuf.nTextured = m_nTextured;
        spImCtx->UpdateSubresource(m_spConstBuf.Get(), 0, nullptr, &constBuf, 0, 0);
        spImCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        spImCtx->IASetVertexBuffers(0, 1, m_spSideVertices.GetAddressOf(), &stride, &offset);
        spImCtx->IASetIndexBuffer(m_spSideIndexes.Get(), DXGI_FORMAT_R32_UINT, 0);
        spImCtx->PSSetShaderResources(0, 1, m_vTex2D[x].spD3D11SRV.GetAddressOf());
        spImCtx->DrawIndexed(static_cast<UINT>(m_vSideIndexes.size()), 0, 0);
    }

    // restore state
    spImCtx->OMSetDepthStencilState(spStateDS.Get(), nStencilRef);
}

void Sphere4::init()
{
	XMFLOAT4 xmf4Color = XMFLOAT4(0.8f, 0.6f, 0.0f, 0.2f);
	XMFLOAT4 xmf4Up(0.0f, 1.0f, 0.0f, 1.0f);

	float fStep = 2.0f * XM_PI / m_nStepsArc;
	for (int i = 0; i <= m_nStepsArc / 4; i++) {
		if (i == 0) {
			// 中心点
			m_vSideVertices.push_back({ XMFLOAT4(0.0f, 0.0f, m_fRadius, 1.0f), xmf4Color, xmf4Up, XMFLOAT2(0.5f, 0.5f) });
		}
		else {
			float fPitch = fStep * i;
            float fs = XMScalarSin(fPitch);
			float fz = m_fRadius * XMScalarCos(fPitch);
			float fr = m_fRadius * fs;
			for (int j = 0; j < m_nStepsArc; j++) {
				float fTheta = fStep * j;
                float fsin = XMScalarSin(fTheta);
                float fcos = XMScalarCos(fTheta);

				float fx = fr * fsin;
				float fy = fr * fcos;
                
                /* 考虑鱼眼(sin模型)变形;而普通标准镜头是tan模型 fr2 = fz * tanf(fPitch)
                   float fr2 = fz * XMScalarSin(fPitch);
                   float fU = 0.5f + 0.5f * fr2 * fsin / fz;
                   float fV = 0.5f - 0.5f * fr2 * fcos * 1.5f / fz;
                   化简后如下: */
                float fU = 0.5f + 0.5f * fsin * fs * 0.7f; // 0.7f是水平补偿系数,经验值
                float fV = 0.5f - 0.5f * fcos * fs * 1.5f; // 垂直方向上视张角120度,水平180度,所以垂直方向上有个1.5f的系数

                /* 考虑鱼眼(2*sin(θ/2)模型)*/
                // fU = 0.5f + 0.5f * 2.0f * XMScalarSin(fPitch / 2.0f) * fsin * 0.6f;
                // fV = 0.5f - 0.5f * 2.0f * XMScalarSin(fPitch / 2.0f) * fcos * 1.5f;

                /* 鱼眼(θ)模型*/
                fU = 0.5f + 0.5f * fPitch * fsin * 0.6f;
                fV = 0.5f - 0.5f * fPitch * fcos * 1.5f;

				m_vSideVertices.push_back({ XMFLOAT4(fx, fy, fz, 1.0f), xmf4Color, xmf4Up, XMFLOAT2(fU, fV) });
				if (i == 1) {
					// 最内圈
					if (j == 0) {
						m_vSideIndexes.push_back(0);
						m_vSideIndexes.push_back(m_nStepsArc);
						m_vSideIndexes.push_back(j + 1);
					}
					else {
						m_vSideIndexes.push_back(0);
						m_vSideIndexes.push_back(j);
						m_vSideIndexes.push_back(j + 1);
					}
				}
				else {
					// 外圈
                    if (j == 0) {
                        m_vSideIndexes.push_back(m_nStepsArc * (i - 1));
                        m_vSideIndexes.push_back(m_nStepsArc * i);
                        m_vSideIndexes.push_back(m_nStepsArc * (i - 1) + 1);
                        
                        m_vSideIndexes.push_back(m_nStepsArc * (i - 1));
                        m_vSideIndexes.push_back(m_nStepsArc * (i - 1) + 1);
                        m_vSideIndexes.push_back(m_nStepsArc * (i - 2) + 1);
                    }
                    else {
                        m_vSideIndexes.push_back(m_nStepsArc * (i - 2) + j);
                        m_vSideIndexes.push_back(m_nStepsArc * (i - 1) + j);
                        m_vSideIndexes.push_back(m_nStepsArc * (i - 1) + j + 1);
                        
                        m_vSideIndexes.push_back(m_nStepsArc * (i - 2) + j);
                        m_vSideIndexes.push_back(m_nStepsArc * (i - 1) + j + 1);
                        m_vSideIndexes.push_back(m_nStepsArc * (i - 2) + j + 1);
                    }
				}
			}
		}
	}
}
