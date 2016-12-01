struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
    return input.Color;
}