cbuffer SkyConstantBuffer : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float4 skyColor;
    uint solidFlag;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 direction : TEXCOORD0;
};

TextureCube skyTexture : register(t0);
SamplerState skySampler : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(lerp(skyColor.rgb, skyTexture.Sample(skySampler, normalize(input.direction)).rgb, solidFlag), 1.0f);
}
