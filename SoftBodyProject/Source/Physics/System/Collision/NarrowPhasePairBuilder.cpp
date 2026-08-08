#include "NarrowPhasePairBuilder.h"


void NarrowPhasePairBuilder::Clear()
{
	sphereSpherePair.clear();
	sphereBoxPair.clear();
	sphereCapsulePair.clear();

	boxSpherePair.clear();
	boxBoxPair.clear();
	boxCapsulePair.clear();

	capsuleSpherePair.clear();
	capsuleBoxPair.clear();
	capsuleCapsulePair.clear();
}

// 球VS球
void NarrowPhasePairBuilder::AddSphereSphere(ColliderID _a, ColliderID _b)
{
	sphereSpherePair.emplace_back(_a, _b);
}
// 球VSボックス
void NarrowPhasePairBuilder::AddSphereBox(ColliderID _sphere, ColliderID _box)
{
	sphereBoxPair.emplace_back(_sphere, _box);
}
// 球VSカプセル
void NarrowPhasePairBuilder::AddSphereCapsule(ColliderID _sphere, ColliderID _capsule)
{
	sphereCapsulePair.emplace_back(_sphere, _capsule);
}

// ボックスVS球
void NarrowPhasePairBuilder::AddBoxSphere(ColliderID _box, ColliderID _sphere)
{
	boxSpherePair.emplace_back(_box, _sphere);
}
// ボックスVSボックス
void NarrowPhasePairBuilder::AddBoxBox(ColliderID _a, ColliderID _b)
{
	boxBoxPair.emplace_back(_a, _b);
}
// ボックスVSカプセル
void NarrowPhasePairBuilder::AddBoxCapsule(ColliderID _box, ColliderID _capsule)
{
	boxCapsulePair.emplace_back(_box, _capsule);
}

// カプセルVS球
void NarrowPhasePairBuilder::AddCapsuleSphere(ColliderID _capsule, ColliderID _sphere)
{
	capsuleSpherePair.emplace_back(_capsule, _sphere);
}
// カプセルVSボックス
void NarrowPhasePairBuilder::AddCapsuleBox(ColliderID _capsule, ColliderID _box)
{
	capsuleBoxPair.emplace_back(_capsule, _box);
}
// カプセルVSカプセル
void NarrowPhasePairBuilder::AddCapsuleCapsule(ColliderID _a, ColliderID _b)
{
	capsuleCapsulePair.emplace_back(_a, _b);
}
