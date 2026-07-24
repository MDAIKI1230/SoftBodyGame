#pragma once

#include <vector>
#include "AABBBroadPhaseCollider.h"

// EntityをColliderIDとする
class AABBBroadPhaseColliderStorage
{
public:
	ColliderID Remove(uint32_t _index)
	{
        if (aabb.empty())
        {
            return ColliderID{ 0,0 };
        }

        size_t last = aabb.size() - 1;
        ColliderID movedId = aabb[last].colliderID;

        if (_index != last)
        {
            aabb[_index] = aabb[last];
            diary[_index] = diary[last];
        }

        aabb.pop_back();
        diary.pop_back();

        return movedId;
	}
public:
	std::vector<AABBBroadPhaseCollider> aabb;
	std::vector<AABBDiaryFlag> diary;
};
