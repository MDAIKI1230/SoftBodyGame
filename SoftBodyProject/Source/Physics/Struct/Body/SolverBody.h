#pragma once

#include "MDMath.h"

#include "BodyID.h"
#include "PhysicsTransformID.h"

struct SolverBody
{
    PhysicsTransformID transformID;
    BodyID bodyID;

    // 通常パラメータ
    Vector3 pastPos;
    Vector3 position;
    Vector3 velocity;
    Quaternion rotation;
    Quaternion pastRot;
    Vector3 angularVelocity;

    float inverseMass{ 0.0f };
    Matrix4x4 localInverseInertiaTensor;
};
