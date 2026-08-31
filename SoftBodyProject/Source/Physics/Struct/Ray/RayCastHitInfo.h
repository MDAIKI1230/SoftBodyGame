#pragma once

#include "MDMath.h"

struct RayCastHitInfo
{
    Vector3 point{};
    Vector3 normal{};

    float distance{};
};
