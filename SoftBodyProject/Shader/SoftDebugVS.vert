#version 430 core

// 構造体
struct Particle
{
    vec4 position;
    vec4 velocity;
};

// SSBO
layout(std430, binding = 0) readonly buffer ParticleBuffer
{
    Particle particles[];
};

// エントリーポイント
void main()
{
    /*
        GLSLの頂点シェーダには通常戻り値を指定せずにoutを使って対応するシェーダに情報を渡す。
        layout(location = 0) out vec4 finalColor;

        void main()
        {
            finalColor = vec4(1.0, 0.2, 0.2, 1.0);
        }

        このようにすると受け取り側のフラグメントシェーダは、

        in vec4 finalColor;
        で受け取る。

        他にも、gl_Positionなどの組み込み出力があるためそれも活用する。

        また、情報の出力は自分でoutで変数作った場合は、次のシェーダ内で有効。
        組み込み変数系は特殊だから場合による
    */
    uint index = uint(gl_VertexID);
    vec3 position = particles[index].position.xyz;

    // 現在の-150～150程度の座標をクリップ座標へ変換
    gl_Position = vec4(
        position.x / 200.0,
        position.y / 150.0,
        0.0,
        1.0
    );

    gl_PointSize = 10.0;
}
