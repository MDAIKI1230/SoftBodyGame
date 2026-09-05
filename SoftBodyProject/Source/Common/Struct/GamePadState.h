#pragma once

#include <array>

#include "GamePadConstants.h"
#include "MDMath.h"

struct GamePadState
{
    std::array<bool, static_cast<std::size_t>(GamePadButton::Count)> buttons{};

    std::array<float, static_cast<std::size_t>(GamePadAxis1D::Count)> axis1D{};

    std::array<Vector2, static_cast<std::size_t>(GamePadAxis2D::Count)> axis2D{};

    bool connected{};
};
