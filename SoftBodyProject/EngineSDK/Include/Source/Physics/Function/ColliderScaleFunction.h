#pragma once

#include "MDMath.h"

namespace ColliderScaleFunction
{
	float SphereRadius(float _radius, const Vector3& _scale);

	float CapsuleRadius(float _radius, const Vector3& _scale);
	float CapsuleHeight(float _height, const Vector3& _scale);

	Vector3 BoxSize(const Vector3& _size, const Vector3& _scale);

	Vector3 Offset(const Vector3& _offset, const Vector3& _scale);
}