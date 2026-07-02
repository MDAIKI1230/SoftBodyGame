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
		points[addIndex] = _contactPoint;
		if (pointCount < 4)
		{
			pointCount++;
		}
		addIndex = (addIndex + 1) % 4;
	}
public:
	ColliderID colliderA;
	ColliderID colliderB;

	Vector3 normal;

	ContactPoint points[4];
	char pointCount{ 0 };
	char addIndex{ 0 };

	bool isCollision;
};
