#pragma once

#include "MDMath.h"

#include "BodyID.h"
#include "PhysicsTransformID.h"

struct SolverBody
{
    PhysicsTransformID transformID;
    BodyID bodyID;

    Vector3 position;
    Vector3 velocity;
    Quaternion rotation;
    Vector3 angularVelocity;

    float inverseMass;
    Matrix4x4 inverseInertiaTensor;
};
