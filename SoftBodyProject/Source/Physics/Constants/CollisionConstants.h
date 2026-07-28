#pragma once

#include <stdint.h>

enum ProjectionAxisType :char
{
	MIN_X,
	MAX_X,
	MIN_Y,
	MAX_Y,
	MIN_Z,
	MAX_Z
};

enum class ColliderType :char
{
	SPHERE,
	BOX,
	COUNT
};

// AABB変更日記
enum AABBChangeDiaryFlag :uint8_t
{
	NONE = 0,
	MAKE = 1,
	TRANSFORM = 1 << 1,
	SHAPE = 1 << 2
};

using AABBDiaryFlag = uint8_t;
