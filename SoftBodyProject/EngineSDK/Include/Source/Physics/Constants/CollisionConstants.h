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
	CAPSULE,
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

namespace CollisionTag
{
	enum Type :uint32_t
	{
		NONE = 1,
		TERRAIN = 1 << 1,   // 地面・壁など
		CARRYABLE = 1 << 2, // 持ち運べる物
		RAGDOLL = 1 << 3,   // ラグドール
	};
}