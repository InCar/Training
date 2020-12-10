
cbuffer ConstantBuffer : register(b0)
{
    matrix WorldViewProjection;
    uint Textured;
}

struct VS_INPUT
{
    float4 Pos : POSITION0;
    float4 Color : COLOR0;
    float4 Normal : NORMAL0;
    float2 Tex : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float4 Normal : NORMAL0;
    float2 Tex : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = input;
    output.Pos = mul(input.Pos, WorldViewProjection);
    return output;
}