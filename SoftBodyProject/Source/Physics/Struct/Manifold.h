#pragma once

#include "CollPair.h"

struct ContactPoint
{
	Vector3 position;
	float penetration;
	float normalImpulse;
};


struct Manifold
{
	int handleA;
	int handleB;

	Vector3 normal;

	ContactPoint points[4];
	int pointCount;
};
