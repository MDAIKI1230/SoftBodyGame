#pragma once

#include "SphereRay.h"
#include "RayCastHitInfo.h"

namespace SphereCastFunction
{
	// SphereRayと球の当たり判定
	bool Sphere(const SphereRay& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Vector3& _scale, float _radius);
	// SphereRayとボックスの当たり判定
	bool Box(const SphereRay& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Quaternion& _rot, const Vector3& _scale, const Vector3& _halfScale);
	// SphereRayとカプセルの当たり判定
	bool Capsule(const SphereRay& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Quaternion& _rot, float _radius, float _height);
	// SphereRayとAABBの当たり判定
	bool AABB(const SphereRay& _ray, const Vector3& _min, const Vector3& _max, float _currentMaxDistance, float& _tEnter);
}