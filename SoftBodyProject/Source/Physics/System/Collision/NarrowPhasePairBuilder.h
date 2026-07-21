#pragma once

#include "CollisionPair.h"

class NarrowPhasePairBuilder
{
public:
	NarrowPhasePairBuilder() = default;
	void Clear();

	void AddSphereSphere(ColliderID _a, ColliderID _b);
	void AddSphereBox(ColliderID _phere, ColliderID _box);
	void AddBoxSphere(ColliderID _box, ColliderID _sphere);
	void AddBoxBox(ColliderID _a, ColliderID _b);
public:
	std::vector<CollisionPair::SphereSpherePair> sphereSpherePair;
	std::vector<CollisionPair::SphereBoxPair> sphereBoxPair;
	std::vector<CollisionPair::BoxSpherePair> boxSpherePair;
	std::vector<CollisionPair::BoxBoxPair> boxBoxPair;
};
