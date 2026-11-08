#pragma once

#include <variant>

#include "MDMath.h"

using InputValue =
std::variant<
    std::monostate,
    bool,
    float,
    Vector2
>;
