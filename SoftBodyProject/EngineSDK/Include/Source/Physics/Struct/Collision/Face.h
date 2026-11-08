#pragma once

#include "MDMath.h"

struct Face
{
	unsigned char pointIndex[3];
	Vector3 normal;
	float distance;
};
