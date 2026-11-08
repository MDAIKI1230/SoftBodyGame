#pragma once

#include "MDMath.h"

#include "ColliderID.h"

struct RayCastQueryHitInfo
{
    Vector3 point{};
    Vector3 normal{};

    float distance{};

    ColliderID colliderID;
};
