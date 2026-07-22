#pragma once

#include <vector>

#include "BodyID.h"
#include "SolveGPUMeta.h"


class ClothStorage
{
public:
    BodyID Remove(uint32_t _index)
    {
        if (id.empty())
        {
            return BodyID{};
        }

        size_t last = id.size() - 1;
        BodyID movedId = id[last];

        if (_index != last)
        {
            id[_index] = id[last];
            meta[_index] = meta[last];
            width[_index] = width[last];
            height[_index] = height[last];
            rowCount[_index] = rowCount[last];
            columnCount[_index] = columnCount[last];
        }

        id.pop_back();
        meta.pop_back();
        height.pop_back();
        width.pop_back();
        rowCount.pop_back();
        columnCount.pop_back();

        return movedId;
    }
public:
    std::vector<BodyID> id;
    std::vector<SolveGPUMeta> meta;
    std::vector<float> width;
    std::vector<float> height;
    std::vector<int > rowCount;
    std::vector<int > columnCount;
};
