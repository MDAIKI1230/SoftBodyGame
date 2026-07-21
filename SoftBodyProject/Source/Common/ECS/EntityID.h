#pragma once

#include <stdint.h>
#include <xhash>

struct EntityID
{
	uint32_t id;

	bool operator==(const EntityID& other) const
	{
		return id == other.id;
	}
};

template<>
struct std::hash<EntityID>
{
    size_t operator()(const EntityID& id) const noexcept
    {
        return std::hash<int>{}(id.id);
    }
};
