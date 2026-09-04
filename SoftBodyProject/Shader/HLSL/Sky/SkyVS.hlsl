cbuffer SkyConstantBuffer : register(b4)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float4 skyColor;
    uint solidFlag;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float4 uv : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 direction : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // 3x3部分だけ使い、モデルの平行移動を無視する
    output.direction = mul(input.pos, (float3x3) world);

    // 3x3部分だけ使い、カメラの平行移動を無視する
    // カメラの回転だけは反映される
    float4 clipPosition = mul(float4(mul(output.direction, (float3x3) view), 1.0f), projection);

    // D3D11の最深部に固定
    clipPosition.z = clipPosition.w;

    output.pos = clipPosition;

    return output;
}
