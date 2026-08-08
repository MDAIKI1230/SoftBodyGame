#pragma once

#include "StorageAccessorsMacros.h"

#include "ColliderID.h"

class CapsuleColliderStorage
{
    MD_STORAGE_READ_ONLY_COLUMN(ColliderID, ID, ids);
    MD_STORAGE_READ_WRITE_COLUMN(float, Height, heights);
    MD_STORAGE_READ_WRITE_COLUMN(float, Radius, radiuses);
public:
    // 追加関数
    void Add(ColliderID _id, const float _height,const float _radius)
    {
        ids.push_back(_id);
        heights.push_back(_height);
        radiuses.push_back(_radius);
    }

    // 除外関数
    ColliderID Remove(size_t _index)
    {
        if (ids.empty())
        {
            return ColliderID(0, 0);
        }

        size_t last = ids.size() - 1;
        ColliderID movedId = ids[last];

        if (_index != last)
        {
            heights[_index] = heights[last];
            radiuses[_index] = radiuses[last];
            ids[_index] = ids[last];
        }

        heights.pop_back();
        radiuses.pop_back();
        ids.pop_back();

        return movedId;
    }
};
