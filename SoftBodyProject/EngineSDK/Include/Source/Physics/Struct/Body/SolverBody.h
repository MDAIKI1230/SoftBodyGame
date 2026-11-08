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
    Vector3 linearFactor{ Vector3::ZERO };
    Vector3 angularFactor{ Vector3::ZERO };

    float inverseMass{ 0.0f };
    Matrix4x4 localInverseInertiaTensor;
};
