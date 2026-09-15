#pragma once

#include <cstdint>

struct CollisionFilter
{
	// 自信がなんなのか
	uint32_t categoryBits{ 1 };
	// 衝突したい相手
	uint32_t collideMask{ UINT32_MAX };

	// --- 必要な場合のみ使おう ---

	// 同じカテゴリ内でも判別用
	uint32_t groupID{ INVALID_COLLISION_GROUP };
	// 同グループ内で判別用
	uint8_t  memberIndex{ 0 };
	// 同グループ内で無視したい相手
	uint64_t ignoreMembers{ 0 };

	static constexpr uint32_t INVALID_COLLISION_GROUP{ 0 };
};