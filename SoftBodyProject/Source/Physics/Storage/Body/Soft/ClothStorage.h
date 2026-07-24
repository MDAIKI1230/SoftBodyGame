#pragma once

#include "StorageAccessorsMacros.h"

#include "BodyID.h"
#include "SolveGPUMeta.h"
#include "ClothUpdateInfo.h"

class ClothStorage
{
    // ID
    MD_STORAGE_READ_ONLY_COLUMN(BodyID, ID, ids);
    // GPU情報
    MD_STORAGE_READ_ONLY_COLUMN(SolveGPUMeta, Meta, metas);
    // 横幅
    MD_STORAGE_READ_WRITE_COLUMN(float, Width, widths);
    // 縦幅
    MD_STORAGE_READ_WRITE_COLUMN(float, Height, heights);
    // 横切り分け数
    MD_STORAGE_READ_WRITE_COLUMN(int, ColumnCount, columnCounts);
    // 縦切り分け数
    MD_STORAGE_READ_WRITE_COLUMN(int, RowCount, rowCounts);
public:
    // Rope作成
    void Create(BodyID _id, const ClothUpdateInfo& _info)
    {
        ids.push_back(_id);
        metas.emplace_back();
        widths.emplace_back(_info.width);
        heights.emplace_back(_info.height);
        columnCounts.emplace_back(_info.columnCount);
        rowCounts.emplace_back(_info.rowCount);
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
            columnCounts[_index] = columnCounts[last];
            rowCounts[_index] = rowCounts[last];
        }

        ids.pop_back();
        metas.pop_back();
        widths.pop_back();
        heights.pop_back();
        columnCounts.pop_back();
        rowCounts.pop_back();

        return movedId;
    }
};
