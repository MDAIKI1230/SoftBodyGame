#pragma once

#include "MDMath.h"
#include "PhysicsTransformID.h"

struct DirectionEndPoint
{
	PhysicsTransformID transformID;
	Vector3 localPosition;
	Vector3 localDirection;
};