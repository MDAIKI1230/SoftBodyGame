#pragma once

#include "StorageAccessorsMacros.h"

#include "ColliderID.h"

class SphereColliderStorage
{
    MD_STORAGE_READ_ONLY_COLUMN(ColliderID, ID, ids);
    MD_STORAGE_READ_WRITE_COLUMN(float, Radius, radiuses);
	MD_STORAGE_READ_WRITE_COLUMN(Vector3, OffsetPosition , offsetPositions);
	MD_STORAGE_READ_WRITE_COLUMN(Quaternion, OffsetRotation , offsetRotations);
public:
    // 追加関数
    void Add(ColliderID _id, float _radius)
    {
        ids.push_back(_id);
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
            radiuses[_index] = radiuses[last];
            ids[_index] = ids[last];
			offsetPositions[_index] = offsetPositions[last];
			offsetRotations[_index] = offsetRotations[last];
        }

        radiuses.pop_back();
        ids.pop_back();
		offsetPositions.pop_back();
		offsetRotations.pop_back();

        return movedId;
	}
};
