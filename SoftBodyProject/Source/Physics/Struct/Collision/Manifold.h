#pragma once

#include "MDMath.h"

#include "BodyID.h"

struct ContactPoint
{
	Vector3 position;
	float penetration;
	float normalImpulse;
};


struct Manifold
{
	BodyID bodyA;
	BodyID bodyB;

	Vector3 normal;

	ContactPoint points[4];
	int pointCount{ 1 };
};
