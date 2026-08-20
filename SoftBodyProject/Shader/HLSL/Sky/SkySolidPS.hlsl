cbuffer SkyConstantBuffer : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float4 skyColor;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(skyColor.rgb, 1.0f);
}
