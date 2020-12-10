
cbuffer ConstantBuffer : register(b0)
{
    matrix WorldViewProjection;
    uint Textured;
    float TexScaleV;
    float TexTransformV;
}

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float4 Normal : NORMAL0;
    float2 Tex : TEXCOORD0;
};

Texture2D gTex2D : register(t0);
SamplerState samLinear : register(s0);

float4 main(VS_OUTPUT input) : SV_Target
{
    if (Textured > 0) {

        return gTex2D.Sample(samLinear, float2(1.0f, TexScaleV) * input.Tex + float2(0.0f, TexTransformV));
        }
    else {
        return input.Color;
    }
}