#pragma once

#include "MDMath.h"

struct SphereRay
{
	Vector3 origin;
	Vector3 direction;
	float radius{ 0.0f };
	float maxDistance{ 0.0f };
};