#pragma once

#include "MDMath.h"

#include "ColliderID.h"

struct BoxBoxContactInfo
{
    ColliderID colliderA;
    ColliderID colliderB;

    Vector3 normal;
    float depth;

    enum Type
    {
        FaceA,
        FaceB,
        EdgeEdge
    } type;

    // FaceAならAの面軸、EdgeEdgeならAの辺軸
    int axisA;
    // FaceBならBの面軸、EdgeEdgeならBの辺軸
    int axisB;
};
