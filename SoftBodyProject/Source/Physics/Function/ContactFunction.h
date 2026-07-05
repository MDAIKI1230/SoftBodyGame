#pragma once

#include "ManifoldFunction.h"

#include "CollisionManifoldBuffer.h"
#include "PhysicsTransformStorage.h"
#include "ColliderStorage.h"

namespace ContactFunction
{
	bool SphereSphere(float _radiusA, float _radiusB, Vector3 _positionA, Vector3 _positionB);
	bool SphereBox(float _radius, Vector3 _scale, Vector3 _positionSphere, Vector3 _positionBox);
	bool BoxBox(const ColliderID& _colliderIDA, const ColliderID& _colliderIDB, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, CollisionManifoldBuffer* _manifoldBuffer);

	bool CheckAxis(BoxBoxContactInfo& _info, BoxBoxContactInfo::Type _type, Vector3& _testAxis, Vector3& _diff, float _length,int _aIndex, int _bIndex);
	float CalcLength(Vector3& _axis, Vector3* _candidateAxisA, Vector3* _candidateAxisB, float* _halfsA, float* _halfsB);
}
