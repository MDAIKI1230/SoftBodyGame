#pragma once

#include <vector>

#include "Constraint.h"

class ConstraintBuffer
{
public:
	void Clear()
	{
		constraints.clear();
	}
public:
	std::vector<Constraint> constraints;
};
