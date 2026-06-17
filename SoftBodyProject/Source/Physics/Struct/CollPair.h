#pragma once

struct CollPair
{
	int a, b;

	bool operator==(const CollPair& other) const
	{
		return a == other.a && b == other.b;
	}
};

template<>
struct std::hash<CollPair>
{
	size_t operator()(const CollPair& p) const
	{
		return std::hash<int>{}(p.a)
			^ (std::hash<int>{}(p.b) << 1);
	}
};