#pragma once

#include "MDMath.h"

// 0～1の範囲で計算する
struct ViewPort
{
    // 左上座標
    Vector2 position{ 0.0f,0.0f };
    // 横幅
    float width{ 1.0f };
    // 縦幅
    float height{ 1.0f };
};
