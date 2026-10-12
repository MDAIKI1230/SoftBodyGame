#pragma once

#include "MDMath.h"

namespace RayCastHelper
{
	// カプセルの時の球の判定関数
	bool CheckSphereBest(
		const Vector3& _origin, const Vector3& _direction, float _maxDistance,
		const Vector3& _center, float _radius, float _heightHalf, Vector3 _outward,
		float& _bestT, Vector3& _bestNormal);
	// 球の二次方程式解く用関数
	bool SolveSphereRoots(
		const Vector3& _origin, const Vector3& _direction,
		const Vector3& _center, float _radius,
		float& _tNear, float& _tFar);
}