#pragma once

#include <vector>

#include "BodyID.h"
#include "SolveGPUMeta.h"

class RopeStorage
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
            length[_index] = length[last];
            segmentCount[_index] = segmentCount[last];
        }

        id.pop_back();
        meta.pop_back();
        length.pop_back();
        segmentCount.pop_back();

        return movedId;
	}
public:
	std::vector<BodyID> id;
	std::vector<SolveGPUMeta> meta;
	std::vector<float> length;
	std::vector<int > segmentCount;
};
