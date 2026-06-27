#pragma once

#include "ColliderTags.h"

namespace CollisionPair
{
	struct BroadPhasePair
	{
		ColliderID a, b;

		bool operator==(const BroadPhasePair& other) const
		{
			return a.index == other.a.index && b.index == other.b.index;
		}
	};

	template<class ATag, class BTag>
	struct CollPair
	{
		ColliderID a, b;

		bool operator==(const CollPair& other) const
		{
			return a.index == other.a.index && b.index == other.b.index;
		}
	};

	using namespace ColliderTag;

	using SphereSpherePair = CollPair<SphereTag, SphereTag>;
	using SphereBoxPair = CollPair<SphereTag, BoxTag>;
	using BoxSpherePair = CollPair<BoxTag, SphereTag>;
	using BoxBoxPair = CollPair<BoxTag, BoxTag>;
}

template<>
struct std::hash<CollisionPair::BroadPhasePair>
{
	size_t operator()(const CollisionPair::BroadPhasePair& p) const
	{
		return std::hash<size_t>{}(p.a.index)
			^ (std::hash<size_t>{}(p.b.index) << 1);
	}
};

template<class ATag, class BTag>
struct std::hash<CollisionPair::CollPair<ATag, BTag>>
{
	size_t operator()(const CollisionPair::CollPair<ATag, BTag>& p) const
	{
		return std::hash<size_t>{}(p.a.index)
			^ (std::hash<size_t>{}(p.b.index) << 1);
	}
};
