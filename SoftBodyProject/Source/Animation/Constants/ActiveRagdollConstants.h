#pragma once

#include <cstdint>

enum class ActiveRagdollControlState : uint8_t
{
	STANDING,
	AIRBORNE,
	FALLEN
};

enum class RagdollGroundState : uint8_t
{
	AIRBORNE,
	WALKABLE,
	STEEP_SLOPE
};