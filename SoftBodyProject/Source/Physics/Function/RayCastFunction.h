#pragma once

#include "Ray.h"
#include "RayCastHitInfo.h"
#include "RayCastQueryHitInfo.h"

namespace RayCastFunction
{
	// Rayと球の当たり判定
	bool Sphere(const Ray& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Vector3& _scale, float _radius);
	// Rayとボックスの当たり判定
	bool Box(const Ray& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Quaternion& _rot, const Vector3& _scale, const Vector3& _halfScale);
	// Rayとカプセルの当たり判定
	bool Capsule(const Ray& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Quaternion& _rot, float _radius, float _height);
	// RayとAABBの当たり判定
	bool AABB(const Ray& _ray, const Vector3& _min, const Vector3& _max, float _currentMaxDistance, float& _tEnter);
}
