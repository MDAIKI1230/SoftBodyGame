#version 430 core

layout(location = 0) out vec4 finalColor;

void main()
{
    // 各点の内部座標を-1～1へ変換
    vec2 point = gl_PointCoord * 2.0 - 1.0;

    float radiusSquared = dot(point, point);

    // 円の外側を破棄
    if (radiusSquared > 1.0)
    {
        discard;
    }

    finalColor = vec4(0.1, 1.0, 0.3, 1.0);
}
