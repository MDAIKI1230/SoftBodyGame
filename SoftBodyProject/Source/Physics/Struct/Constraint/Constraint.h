#pragma once

#include "MDMath.h"

struct Constraint
{
	float bias;
	float cfm;
	float constraintError;
	Vector3 jacobian;
};
