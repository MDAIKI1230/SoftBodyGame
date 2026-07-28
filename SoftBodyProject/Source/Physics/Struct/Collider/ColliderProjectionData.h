#pragma once

#include <cstdint>

#include "MDMath.h"

struct ColliderProjectionData
{
    Vector3 min;
    Vector3 max;

    uint32_t endpointIndex[6]{};
};
