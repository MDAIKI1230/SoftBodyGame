#pragma once

#include "GenerationalID.h"
#include <xhash>

struct EntityTag;

using EntityID = GenerationalID<EntityTag>;

template<>
struct std::hash<EntityID>
{
    size_t operator()(const EntityID& id) const noexcept
    {
        return std::hash<size_t>{}(id.GetIndex())
            ^ (std::hash<size_t>{}(id.GetGeneration()) << 1);
    }
};
