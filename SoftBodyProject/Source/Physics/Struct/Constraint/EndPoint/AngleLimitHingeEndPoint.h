#pragma once

#include "MDMath.h"

#include "physicsTransformID.h"

struct AngleLimitHingeEndPoint
{
	PhysicsTransformID transformID;

	// Bodyローカル空間
	Vector3 localPosition;
	Vector3 localAxis;

	// localAxisと直交する、角度0の基準方向
	Vector3 localReferenceDirection;
};