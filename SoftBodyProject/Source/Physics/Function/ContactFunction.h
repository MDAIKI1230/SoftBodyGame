#pragma once

#include "MDMath.h"

namespace ContactFunction
{
	bool ContactSphereSphere(float _radiusA, float _radiusB, Vector3 _positionA, Vector3 _positionB);
	bool ContactSphereBox(float _radius, Vector3 _scale, Vector3 _positionSphere, Vector3 _positionBox);
	bool ContactBoxBox(Vector3 _scaleA, Vector3 _positionA, Vector3 _scaleB, Vector3 _positionB);
}
