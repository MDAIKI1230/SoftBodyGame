#pragma once

#include "ColliderTags.h"

template<class ATag,class BTag>
struct CollPair
{
	int a, b;

	bool operator==(const CollPair& other) const
	{
		return a == other.a && b == other.b;
	}
};

template<class ATag, class BTag>
struct std::hash<CollPair<ATag, BTag>>
{
	size_t operator()(const CollPair<ATag, BTag>& p) const
	{
		return std::hash<int>{}(p.a)
			^ (std::hash<int>{}(p.b) << 1);
	}
};

using namespace ColliderTag;

using SphereSpherePair = CollPair<SphereTag, SphereTag>;
