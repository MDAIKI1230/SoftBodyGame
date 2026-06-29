#pragma once

#include "MDMath.h"

#include "ColliderID.h"

struct ContactPoint
{
	Vector3 position;
	float penetration;
	float normalImpulse;
};


struct Manifold
{
	ColliderID colliderA;
	ColliderID colliderB;

	Vector3 normal;

	ContactPoint points[4];
	int pointCount{ 1 };
};
