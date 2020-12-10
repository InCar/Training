#pragma once

struct Space
{
    DirectX::XMMATRIX mWorld;
    DirectX::XMMATRIX mView;
    DirectX::XMMATRIX mProjection;
};

struct ConstantBuffer
{
    DirectX::XMMATRIX mWorldViewProjection;
    uint nTextured;
    float fTexScaleV = 1.0f;
    float fTexTransformV = 0.0f;
};

struct Texture2DResource
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> spD3D11Tex2D;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> spD3D11SRV;
};

struct ColorPoint
{
    DirectX::XMFLOAT4 Point;
    DirectX::XMFLOAT4 Color;
    DirectX::XMFLOAT4 Normal;
    DirectX::XMFLOAT2 Tex;
};

class Geometry
{
public:
    virtual HRESULT CreateD3DResources(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev);
    virtual void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& spImCtx, const Space& space) = 0;
protected:
    // shader
    DWORD m_dwVS, m_dwPS;
    std::unique_ptr<BYTE> m_uptrVS, m_uptrPS;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_spVS;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_spPS;

    // constant buffer
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_spConstBuf;
    // input layout
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_spIL;
    // sampler state
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_spSamplerState;

    // 加载shader
    virtual HRESULT loadShader(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev);
    // create constant buffer
    virtual HRESULT createConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev);
    // create input layout
    virtual HRESULT createInputLayout(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev);
    // create sampler state
    virtual HRESULT createSamplerState(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev);
protected:
    // load texture
    static HRESULT loadTexture2D(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev,
        const std::string& strPathImage,
        Microsoft::WRL::ComPtr<ID3D11Texture2D>& spTex2D,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& spSRV);
    static HRESULT loadTexture2D(Microsoft::WRL::ComPtr<ID3D11Device>& spD3D11Dev,
        const cv::Mat& image,
        Microsoft::WRL::ComPtr<ID3D11Texture2D>& spTex2D,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& spSRV);
};