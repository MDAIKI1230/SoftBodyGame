#pragma once

#include "MDMath.h"

struct Constraint
{
	float constraintError;
	Vector3 jacobian;
};
