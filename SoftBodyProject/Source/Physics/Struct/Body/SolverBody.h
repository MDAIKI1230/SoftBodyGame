#pragma once

#include "MDMath.h"

#include "BodyID.h"
#include "PhysicsTransformID.h"

struct SolverBody
{
    PhysicsTransformID transformID;
    BodyID bodyID;

    Vector3 pastPos;
    Vector3 position;
    Vector3 velocity;
    Quaternion rotation;
    Quaternion pastRot;
    Vector3 angularVelocity;

    float mass;

    float inverseMass;
    Matrix4x4 inverseInertiaTensor;
};
