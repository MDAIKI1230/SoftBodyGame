#pragma once

#include "StorageAccessorsMacros.h"

#include "BodyID.h"
#include "SolveGPUMeta.h"
#include "SoftBodyUpdateInfo.h"

class SoftBodyStorage
{
    // ID
    MD_STORAGE_READ_ONLY_COLUMN(BodyID, ID, ids);
    // GPU情報
    MD_STORAGE_READ_ONLY_COLUMN(SolveGPUMeta, Meta, metas);
    // 横幅
    MD_STORAGE_READ_WRITE_COLUMN(float, Width, widths);
    // 縦幅
    MD_STORAGE_READ_WRITE_COLUMN(float, Height, heights);
    // 奥行き
    MD_STORAGE_READ_WRITE_COLUMN(float, Depth, depths);
    // 横切り分け数
    MD_STORAGE_READ_WRITE_COLUMN(int, SegmentCountX, segmentCountsX);
    // 縦切り分け数
    MD_STORAGE_READ_WRITE_COLUMN(int, SegmentCountY, segmentCountsY);
    // 奥行き分け数
    MD_STORAGE_READ_WRITE_COLUMN(int, SegmentCountZ, segmentCountsZ);
public:
    // SoftBody作成
    void Create(BodyID _id, const SoftBodyUpdateInfo& _info)
    {
        ids.push_back(_id);
        metas.emplace_back();
        widths.emplace_back(_info.width);
        heights.emplace_back(_info.height);
        depths.emplace_back(_info.depth);
        segmentCountsX.emplace_back(_info.segmentCountX);
        segmentCountsY.emplace_back(_info.segmentCountY);
        segmentCountsZ.emplace_back(_info.segmentCountZ);
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
            widths[_index] = widths[last];
            heights[_index] = heights[last];
            depths[_index] = depths[last];
            segmentCountsX[_index] = segmentCountsX[last];
            segmentCountsY[_index] = segmentCountsY[last];
            segmentCountsZ[_index] = segmentCountsZ[last];
        }

        ids.pop_back();
        metas.pop_back();
        widths.pop_back();
        heights.pop_back();
        depths.pop_back();
        segmentCountsX.pop_back();
        segmentCountsY.pop_back();
        segmentCountsZ.pop_back();

        return movedId;
    }
};
