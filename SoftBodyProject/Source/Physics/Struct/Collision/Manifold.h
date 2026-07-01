#pragma once

#include "MDMath.h"

#include "ColliderID.h"

struct ContactPoint
{
	Vector3 position;
	float penetration;
};


struct Manifold
{
public:
	void AddPoints(const ContactPoint& _contactPoint)
	{
		points[pointCount] = _contactPoint;
		pointCount = (pointCount + 1) % 4;
	}
public:
	ColliderID colliderA;
	ColliderID colliderB;

	Vector3 normal;

	ContactPoint points[4];
	int pointCount{ 0 };

	bool isCollision;
};
