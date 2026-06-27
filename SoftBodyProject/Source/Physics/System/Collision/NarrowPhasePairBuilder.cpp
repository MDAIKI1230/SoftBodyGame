#include "NarrowPhasePairBuilder.h"


void NarrowPhasePairBuilder::Clear()
{
	sphereSpherePair.clear();
	sphereBoxPair.clear();
	boxBoxPair.clear();
}

void NarrowPhasePairBuilder::AddSphereSphere(ColliderID _a, ColliderID _b)
{
	sphereSpherePair.emplace_back(_a, _b);
}

void NarrowPhasePairBuilder::AddSphereBox(ColliderID _sphere, ColliderID _box)
{
	sphereBoxPair.emplace_back(_sphere, _box);
}

void NarrowPhasePairBuilder::AddBoxSphere(ColliderID _box, ColliderID _sphere)
{
	boxSpherePair.emplace_back(_box, _sphere);
}

void NarrowPhasePairBuilder::AddBoxBox(ColliderID _a, ColliderID _b)
{
	boxBoxPair.emplace_back(_a, _b);
}
