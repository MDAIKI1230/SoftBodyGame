#pragma once

#include "MDMath.h"

struct Ray
{
    Vector3 origin;
    Vector3 direction;
    float maxDistance{ 0.0f };
};
