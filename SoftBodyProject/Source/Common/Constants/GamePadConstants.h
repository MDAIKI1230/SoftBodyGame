#pragma once

#pragma once

// boolを返す入力
enum class GamePadButton : int
{
    BUTTON_SOUTH,       // Xbox:A / PS:Cross
    BUTTON_EAST,        // Xbox:B / PS:Circle
    BUTTON_WEST,        // Xbox:X / PS:Square
    BUTTON_NORTH,       // Xbox:Y / PS:Triangle

    LEFT_SHOULDER,
    RIGHT_SHOULDER,

    LEFT_STICK_PRESS,
    RIGHT_STICK_PRESS,

    DPAD_UP,
    DPAD_DOWN,
    DPAD_LEFT,
    DPAD_RIGHT,

    SELECT,
    START,
    GUIDE,

    Count
};

// floatを返す入力
enum class GamePadAxis1D : int
{
    LEFT_TRIGGER,
    RIGHT_TRIGGER,
    Count
};

// Vector2を返す入力
enum class GamePadAxis2D : int
{
    LEFT_STICK,
    RIGHT_STICK,
    Count
};

using GamePadConstants = std::variant<
    GamePadButton,
    GamePadAxis1D,
    GamePadAxis2D,
    std::monostate
>;
