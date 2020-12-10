#include "stdafx.h"
#include "Geometry.h"
#include "Karmeliet.h"

using namespace std;
using namespace Microsoft::WRL;

HRESULT Geometry::CreateD3DResources(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev)
{
    HRESULT hr = createConstantBuffer(spD3D11Dev);
    if (FAILED(hr)) return hr;

    hr = loadShader(spD3D11Dev);
    if (FAILED(hr)) return hr;

    hr = createInputLayout(spD3D11Dev);
    if (FAILED(hr)) return hr;

    hr = createSamplerState(spD3D11Dev);
    if (FAILED(hr)) return hr;

    return S_OK;
}

HRESULT Geometry::loadShader(ComPtr<ID3D11Device>& spD3D11Dev)
{
    const int size = MAX_PATH;
    wchar_t buf[size];
    GetModuleFileName(NULL, buf, size);
    wchar_t* pLast = wcsrchr(buf, L'\\');
    if (pLast == NULL) return E_FAIL;

    auto fnLoad = [](const wchar_t* path, unique_ptr<BYTE>& uptrShader, DWORD& dwSize)->HRESULT {
        HANDLE hCSO = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (hCSO == INVALID_HANDLE_VALUE) return E_HANDLE;
        dwSize = GetFileSize(hCSO, NULL);
        uptrShader = unique_ptr<BYTE>(new BYTE[dwSize]);
        ReadFile(hCSO, uptrShader.get(), dwSize, &dwSize, NULL);
        CloseHandle(hCSO);
        return S_OK;
    };

    wcscpy_s(pLast + 1, size - ((pLast + 1) - buf), L"VS.cso");
    fnLoad(buf, m_uptrVS, m_dwVS);
    HRESULT hr = spD3D11Dev->CreateVertexShader(m_uptrVS.get(), m_dwVS, NULL, &m_spVS);
    if (FAILED(hr)) return hr;

    wcscpy_s(pLast + 1, size - ((pLast + 1) - buf), L"PS.cso");
    fnLoad(buf, m_uptrPS, m_dwPS);
    hr = spD3D11Dev->CreatePixelShader(m_uptrPS.get(), m_dwPS, NULL, &m_spPS);
    if (FAILED(hr)) return hr;

    return S_OK;
}

HRESULT Geometry::createConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev)
{
    // shader constant buffer
    D3D11_BUFFER_DESC descConstBuf;
    ZeroMemory(&descConstBuf, sizeof(D3D11_BUFFER_DESC));
    descConstBuf.Usage = D3D11_USAGE_DEFAULT;
    descConstBuf.ByteWidth = sizeof(ConstantBuffer);
    descConstBuf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    HRESULT hr = spD3D11Dev->CreateBuffer(&descConstBuf, NULL, &m_spConstBuf);
    return hr;
}

HRESULT Geometry::createInputLayout(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev)
{
    // input layout
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL"  , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    HRESULT hr = spD3D11Dev->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), m_uptrVS.get(), m_dwVS, &m_spIL);
    return hr;
}

HRESULT Geometry::createSamplerState(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev)
{
    D3D11_SAMPLER_DESC descSampler = {
        D3D11_FILTER_MIN_MAG_MIP_LINEAR,
        D3D11_TEXTURE_ADDRESS_BORDER,
        D3D11_TEXTURE_ADDRESS_BORDER,
        D3D11_TEXTURE_ADDRESS_BORDER,
        0.0f,
        1,
        D3D11_COMPARISON_NEVER,
        {0.1f, 0.1f, 0.1f, 0.0f},
        -FLT_MAX,
        +FLT_MAX
    };
    HRESULT hr = spD3D11Dev->CreateSamplerState(&descSampler, m_spSamplerState.GetAddressOf());
    return hr;
}

HRESULT Geometry::loadTexture2D(ComPtr<ID3D11Device>& spD3D11Dev, const std::string& strPathImage,
    ComPtr<ID3D11Texture2D>& spTex2D, ComPtr<ID3D11ShaderResourceView>& spSRV)
{
    Karmeliet kaImage;
    kaImage.LoadTexture(strPathImage);

    HRESULT hr = kaImage.CreateTexture2D(spD3D11Dev, spTex2D);
    if (FAILED(hr)) return hr;

    hr = spD3D11Dev->CreateShaderResourceView(spTex2D.Get(), nullptr, spSRV.GetAddressOf());
    if (FAILED(hr)) return hr;

    return S_OK;
}

HRESULT Geometry::loadTexture2D(ComPtr<ID3D11Device>& spD3D11Dev, const cv::Mat& image,
    ComPtr<ID3D11Texture2D>& spTex2D, ComPtr<ID3D11ShaderResourceView>& spSRV)
{
    Karmeliet kaImage;
    kaImage.LoadTextureRGBA(image);

    HRESULT hr = kaImage.CreateTexture2D(spD3D11Dev, spTex2D);
    if (FAILED(hr)) return hr;

    hr = spD3D11Dev->CreateShaderResourceView(spTex2D.Get(), nullptr, spSRV.GetAddressOf());
    if (FAILED(hr)) return hr;

    return S_OK;
}