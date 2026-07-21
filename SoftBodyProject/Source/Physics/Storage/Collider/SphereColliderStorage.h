#pragma once

#include <vector>

#include "ColliderID.h"

class SphereColliderStorage
{
public:
	ColliderID Remove(size_t _index)
	{
        if (id.empty())
        {
            return ColliderID(0, 0);
        }

        size_t last = id.size() - 1;
        ColliderID movedId = id[last];

        if (_index != last)
        {
            radius[_index] = radius[last];
            id[_index] = id[last];
        }

        radius.pop_back();
        id.pop_back();

        return movedId;
	}
public:
	std::vector<float> radius;
	std::vector<ColliderID> id;
};
