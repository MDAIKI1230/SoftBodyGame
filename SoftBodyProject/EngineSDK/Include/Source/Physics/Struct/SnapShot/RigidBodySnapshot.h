#pragma once

#include "MDMath.h"

#include "BodyConstants.h"
#include "BodyID.h"

struct RigidBodySnapshot
{
    BodyID id;

    Vector3 previousPosition;
    Quaternion previousRotation;

    Vector3 currentPosition;
    Quaternion currentRotation;

    Vector3 velocity;
    Vector3 angularVelocity;

    uint64_t writtenTick;
};
