#pragma once

#include <vector>
#include "AABBBroadPhaseCollider.h"

class AABBBroadPhaseColliderStorage
{
public:
    // AABB取得関数
    const AABBBroadPhaseCollider& GetAABB(uint32_t _index)
    {
        return aabb[_index];
    }
    // Diary取得関数
    AABBDiaryFlag GetDiary(uint32_t _index)
    {
        return diary[_index];
    }
    // AABB参照返し関数
    AABBBroadPhaseCollider& EditAABB(uint32_t _index)
    {
        return aabb[_index];
    }
    // Diary参照返し関数
    AABBDiaryFlag& EditDiary(uint32_t _index)
    {
        return diary[_index];
    }
    // AABBの数
    uint32_t Count()
    {
        return static_cast<uint32_t>(aabb.size());
    }
    // 追加関数
    void Add(const AABBBroadPhaseCollider& _aabb, AABBDiaryFlag _diary)
    {
        aabb.push_back(_aabb);
        diary.push_back(_diary);
    }

    // 除外関数
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
private:
	std::vector<AABBBroadPhaseCollider> aabb;
	std::vector<AABBDiaryFlag> diary;
};
