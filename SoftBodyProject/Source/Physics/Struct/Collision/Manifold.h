#pragma once

#include "MDMath.h"

#include "ColliderID.h"

struct ContactPoint
{
	Vector3 positionLocalA;
	Vector3 positionLocalB;
	float penetration{ 0.0f };
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

	void Clear()
	{
		pointCount = 0;
		addIndex = 0;
	}
public:
	ColliderID colliderA;
	ColliderID colliderB;

	Vector3 normal;

	ContactPoint points[4];

	char pointCount{ 0 };
private:
	char addIndex{ 0 };
};
