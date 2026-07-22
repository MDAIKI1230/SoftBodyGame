#pragma once

#include <vector>

#include "BodyID.h"
#include "SolveGPUMeta.h"

class SoftBodyStorage
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
            depth[_index] = depth[last];
            columnCountX[_index] = columnCountX[last];
            columnCountY[_index] = columnCountY[last];
            columnCountZ[_index] = columnCountZ[last];
        }

        id.pop_back();
        meta.pop_back();
        width.pop_back();
        height.pop_back();
        depth.pop_back();
        columnCountX.pop_back();
        columnCountY.pop_back();
        columnCountZ.pop_back();

        return movedId;
    }
public:
    std::vector<BodyID> id;
    std::vector<SolveGPUMeta> meta;
    std::vector<float> width;
    std::vector<float> height;
    std::vector<float> depth;
    std::vector<int > columnCountX;
    std::vector<int > columnCountY;
    std::vector<int > columnCountZ;
};
