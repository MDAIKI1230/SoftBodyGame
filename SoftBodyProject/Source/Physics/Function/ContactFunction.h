#pragma once

#include "ManifoldFunction.h"

#include "CollisionManifoldBuffer.h"
#include "PhysicsTransformStorage.h"
#include "ColliderStorage.h"

namespace ContactFunction
{
	bool SphereSphere(ColliderID _colliderA, ColliderID _colliderB, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, CollisionManifoldBuffer* _manifoldBuffer);
	bool SphereBox(ColliderID _colliderA, ColliderID _colliderB, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, CollisionManifoldBuffer* _manifoldBuffer);
	bool BoxBox(ColliderID _colliderSpher, ColliderID _colliderBox, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, CollisionManifoldBuffer* _manifoldBuffer);

	bool CheckAxis(BoxBoxContactInfo& _info, BoxBoxContactInfo::Type _type, const Vector3& _testAxis, const Vector3& _diff, float _length,int _aIndex, int _bIndex);
	float CalcLength(const Vector3& _axis, const Vector3* _candidateAxisA, const Vector3* _candidateAxisB, const float* _halfsA, const float* _halfsB);
}
