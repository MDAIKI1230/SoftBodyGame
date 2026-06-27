#pragma once

#include "MDMath.h"

struct ContactPoint
{
	Vector3 position;
	float penetration;
	float normalImpulse;
};


struct Manifold
{
	EntityID handleA;
	EntityID handleB;

	Vector3 normal;

	ContactPoint points[4];
	int pointCount;
};
