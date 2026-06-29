#pragma once

#include "MDMath.h"

#include "BodyID.h"
#include "PhysicsTransformID.h"

struct SolverBody
{
    PhysicsTransformID transformID;
    BodyID bodyID;

    Vector3 position;
    Quaternion rotation;
    Vector3 velocity;
    Vector3 angularVelocity;

    float inverseMass;
    Matrix4x4 inverseInertia;

    bool writeBack;
};
