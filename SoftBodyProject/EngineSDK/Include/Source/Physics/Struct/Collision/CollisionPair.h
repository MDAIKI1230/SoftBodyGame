#pragma once

#include "ColliderTags.h"

namespace CollisionPair
{
	struct Pair
	{
		ColliderID a, b;

		bool operator==(const Pair& other) const
		{
			return a == other.a && b == other.b;
		}
	};

	template<class ATag, class BTag>
	struct CollPair
	{
		ColliderID a, b;

		bool operator==(const CollPair& other) const
		{
			return a == other.a && b == other.b;
		}
	};

	using SphereSpherePair = CollPair<SphereTag, SphereTag>;
	using SphereBoxPair = CollPair<SphereTag, BoxTag>;
	using SphereCapsulePair = CollPair<SphereTag, CapsuleTag>;

	using BoxSpherePair = CollPair<BoxTag, SphereTag>;
	using BoxBoxPair = CollPair<BoxTag, BoxTag>;
	using BoxCapsulePair = CollPair<BoxTag, CapsuleTag>;

	using CapsuleSpherePair = CollPair<CapsuleTag, SphereTag>;
	using CapsuleBoxPair = CollPair<CapsuleTag, BoxTag>;
	using CapsuleCapsulePair = CollPair<CapsuleTag, CapsuleTag>;
}

template<>
struct std::hash<CollisionPair::Pair>
{
	size_t operator()(const CollisionPair::Pair& p) const
	{
		return std::hash<size_t>{}(p.a.GetIndex())
			^ (std::hash<size_t>{}(p.b.GetIndex()) << 1);
	}
};

template<class ATag, class BTag>
struct std::hash<CollisionPair::CollPair<ATag, BTag>>
{
	size_t operator()(const CollisionPair::CollPair<ATag, BTag>& p) const
	{
		return std::hash<size_t>{}(p.a.GetIndex())
			^ (std::hash<size_t>{}(p.b.GetIndex()) << 1);
	}
};
