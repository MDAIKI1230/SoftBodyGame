#pragma once

#include <variant>

// boolを返す入力
enum class MouseButton : int
{
    LEFT,
    RIGHT,
    MIDDLE,

    BUTTON_4,
    BUTTON_5,
    BUTTON_6,
    BUTTON_7,
    BUTTON_8,

    Count
};

// floatを返す入力
enum class MouseAxis1D : int
{
    SCROLL_X,
    SCROLL_Y,

    Count
};

// Vector2を返す入力
enum class MouseAxis2D : int
{
    // ウィンドウ内のカーソル座標
    POSITION,

    // 前フレームからの移動量
    DELTA,

    // 横・縦ホイール入力
    SCROLL,

    Count
};

using MouseConstants = std::variant<
    MouseButton,
    MouseAxis1D,
    MouseAxis2D,
    std::monostate
>;
