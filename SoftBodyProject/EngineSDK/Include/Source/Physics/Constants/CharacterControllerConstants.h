#pragma once

#include <cstdint>

enum class CharacterGroundState : uint8_t
{
	AIRBORNE,
	WALKABLE,
	JUMPING,
	STEEP_SLOPE
};
