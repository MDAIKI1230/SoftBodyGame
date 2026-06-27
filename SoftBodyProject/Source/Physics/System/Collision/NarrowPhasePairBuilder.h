#pragma once

#include "CollisionPair.h"

class NarrowPhasePairBuilder
{
public:
	NarrowPhasePairBuilder() = default;
	void Clear();

	void AddSphereSphere(ColliderID a, ColliderID b);
	void AddSphereBox(ColliderID sphere, ColliderID box);
	void AddBoxSphere(ColliderID sphere, ColliderID box);
	void AddBoxBox(ColliderID a, ColliderID b);
public:
	std::vector<CollisionPair::SphereSpherePair> sphereSpherePair;
	std::vector<CollisionPair::SphereBoxPair> sphereBoxPair;
	std::vector<CollisionPair::BoxSpherePair> boxSpherePair;
	std::vector<CollisionPair::BoxBoxPair> boxBoxPair;
};
