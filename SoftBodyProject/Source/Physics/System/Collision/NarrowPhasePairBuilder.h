#pragma once

#include "CollisionPair.h"

class NarrowPhasePairBuilder
{
public:
	NarrowPhasePairBuilder() = default;
	void Clear();

	// 球VS球
	void AddSphereSphere(ColliderID _a, ColliderID _b);
	// 球VSボックス
	void AddSphereBox(ColliderID _sphere, ColliderID _box);
	// 球VSカプセル
	void AddSphereCapsule(ColliderID _sphere, ColliderID _capsule);

	// ボックスVS球
	void AddBoxSphere(ColliderID _box, ColliderID _sphere);
	// ボックスVSボックス
	void AddBoxBox(ColliderID _a, ColliderID _b);
	// ボックスVSカプセル
	void AddBoxCapsule(ColliderID _box, ColliderID _capsule);

	// カプセルVS球
	void AddCapsuleSphere(ColliderID _capsule, ColliderID _sphere);
	// カプセルVSボックス
	void AddCapsuleBox(ColliderID _capsule, ColliderID _box);
	// カプセルVSカプセル
	void AddCapsuleCapsule(ColliderID _a, ColliderID _b);
public:
	// 球球
	std::vector<CollisionPair::SphereSpherePair> sphereSpherePair;
	// 球ボックス
	std::vector<CollisionPair::SphereBoxPair> sphereBoxPair;
	// 球カプセル
	std::vector<CollisionPair::SphereCapsulePair> sphereCapsulePair;

	// ボックス球
	std::vector<CollisionPair::BoxSpherePair> boxSpherePair;
	// ボックスボックス
	std::vector<CollisionPair::BoxBoxPair> boxBoxPair;
	// ボックスカプセル
	std::vector<CollisionPair::BoxCapsulePair> boxCapsulePair;

	// カプセル球
	std::vector<CollisionPair::CapsuleSpherePair> capsuleSpherePair;
	// カプセルボックス
	std::vector<CollisionPair::CapsuleBoxPair> capsuleBoxPair;
	// カプセルカプセル
	std::vector<CollisionPair::CapsuleCapsulePair> capsuleCapsulePair;
};
