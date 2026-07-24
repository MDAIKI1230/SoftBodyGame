#pragma once

#include "StorageAccessorsMacros.h"

#include "BodyID.h"
#include "SolveGPUMeta.h"
#include "RopeUpdateInfo.h"

class RopeStorage
{
    // ID
    MD_STORAGE_READ_ONLY_COLUMN(BodyID, ID, ids);
    // GPU情報
    MD_STORAGE_READ_WRITE_COLUMN(SolveGPUMeta, Meta, metas);
    // 長さ
    MD_STORAGE_READ_WRITE_COLUMN(float, Length, lengths);
    // 切り分け数
    MD_STORAGE_READ_WRITE_COLUMN(int, SegmentCount, segmentCounts);
public:
    // Rope作成
    void CreateRope(BodyID _id, const RopeUpdateInfo& _info)
    {
        ids.push_back(_id);
        metas.emplace_back();
        lengths.emplace_back(_info.length);
        segmentCounts.emplace_back(_info.segmentCount);
    }

    // 破棄
    BodyID Remove(uint32_t _index)
	{
        if (ids.empty())
        {
            return BodyID{};
        }

        size_t last = ids.size() - 1;
        BodyID movedId = ids[last];

        if (_index != last)
        {
            ids[_index] = ids[last];
            metas[_index] = metas[last];
            lengths[_index] = lengths[last];
            segmentCounts[_index] = segmentCounts[last];
        }

        ids.pop_back();
        metas.pop_back();
        lengths.pop_back();
        segmentCounts.pop_back();

        return movedId;
	}
};
