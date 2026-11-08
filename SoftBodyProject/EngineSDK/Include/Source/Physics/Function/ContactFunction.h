#pragma once

#include "CollisionManifoldBuffer.h"
#include "PhysicsTransformStorage.h"
#include "ColliderStorage.h"

#include "ContactInfo.h"
#include "BoxBoxContactInfo.h"

namespace ContactFunction
{
	// 球 VS 球
	bool SphereSphere(
		ColliderID _colliderA, ColliderID _colliderB,
		ContactInfo& _outputContactA, ContactInfo& _outputContactB,
		ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage,
		CollisionManifoldBuffer* _manifoldBuffer);
	// 球 VS ボックス
	bool SphereBox(
		ColliderID _colliderSphere, ColliderID _colliderBox,
		ContactInfo& _outputContactSphere, ContactInfo& _outputContactBox,
		ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage,
		CollisionManifoldBuffer* _manifoldBuffer);
	// ボックス VS ボックス
	bool BoxBox(
		ColliderID _colliderA, ColliderID _colliderB,
		ContactInfo& _outputContactA, ContactInfo& _outputContactB,
		ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage,
		CollisionManifoldBuffer* _manifoldBuffer);
	// カプセル VS カプセル
	bool CapsuleCapsule(
		ColliderID _colliderA, ColliderID _colliderB,
		ContactInfo& _outputContactA, ContactInfo& _outputContactB,
		ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage,
		CollisionManifoldBuffer* _manifoldBuffer);
	// ボックス VS カプセル
	bool BoxCapsule(
		ColliderID _colliderBox, ColliderID _colliderCapsule,
		ContactInfo& _outputContactBox, ContactInfo& _outputContactCapsule,
		ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage,
		CollisionManifoldBuffer* _manifoldBuffer);
	// 球 VS カプセル
	bool SphereCapsule(
		ColliderID _colliderSphere, ColliderID _colliderCapsule,
		ContactInfo& _outputContactSphere, ContactInfo& _outputContactCapsule,
		ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage,
		CollisionManifoldBuffer* _manifoldBuffer);

	bool CheckAxis(BoxBoxContactInfo& _info, BoxBoxContactInfo::Type _type, const Vector3& _testAxis, const Vector3& _diff, float _length,int _aIndex, int _bIndex);
	float CalcLength(const Vector3& _axis, const Vector3* _candidateAxisA, const Vector3* _candidateAxisB, const float* _halfsA, const float* _halfsB);
}
