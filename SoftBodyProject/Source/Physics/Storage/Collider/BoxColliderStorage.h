#pragma once

#pragma once

#include <vector>

#include "ColliderID.h"

class BoxColliderStorage
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
            scale[_index] = scale[last];
            id[_index] = id[last];
        }

        scale.pop_back();
        id.pop_back();

        return movedId;
	}
public:
	std::vector<Vector3> scale;
	std::vector<ColliderID> id;
};
