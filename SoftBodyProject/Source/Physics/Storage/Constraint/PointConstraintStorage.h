#pragma once

#include <vector>

#include "PointConstraint.h"

class PointConstraintStorage
{
public:
	ConstraintID Remove(uint32_t _index)
	{
        if (id.empty())
        {
            return ConstraintID{ 0, 0 };
        }

        size_t last = id.size() - 1;
        ConstraintID movedId = id[last];

        if (_index != last)
        {
            constraints[_index] = constraints[last];
            id[_index] = id[last];
        }

        constraints.pop_back();
        id.pop_back();

        return movedId;
	}
public:
    std::vector<ConstraintID> id;
	std::vector<PointConstraint> constraints;
};
