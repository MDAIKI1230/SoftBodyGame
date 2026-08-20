cbuffer SkyConstantBuffer : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float4 skyColor;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float4 diffuse : COLOR0;
    float4 specular : COLOR1;
    float4 uv0 : TEXCOORD0;
    float4 uv1 : TEXCOORD1;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // 3x3部分だけ使い、モデルの平行移動を無視する
    float3 worldDirection =
        mul(input.pos, (float3x3) world);

    // 3x3部分だけ使い、カメラの平行移動を無視する
    // カメラの回転だけは反映される
    float3 viewDirection =
        mul(worldDirection, (float3x3) view);

    float4 clipPosition =
        mul(float4(viewDirection, 1.0f), projection);

    // D3D11の最深部に固定
    clipPosition.z = clipPosition.w;

    output.pos = clipPosition;
    output.uv = input.uv0.xy;

    return output;
}
