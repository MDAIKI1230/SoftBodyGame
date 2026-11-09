#pragma once

#include <cstdint>

namespace CollisionTag
{
	enum Type : std::uint32_t
	{
		NONE = 1,
		RAGDOLL = 1 << 1, // ラグドール
	};
}