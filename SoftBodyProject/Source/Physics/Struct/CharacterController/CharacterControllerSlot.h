#pragma once

#include <cstdint>

#include "EntityID.h"

struct CharacterControllerSlot
{
public:
	CharacterControllerSlot(uint32_t _denseIndex, EntityID _ownerEntity) :
		denseIndex{ _denseIndex },
		ownerEntity{ _ownerEntity }
	{
	}

public:
	// 世代
	uint32_t generation{ 1 };
	// 生存フラグ
	bool alive{ true };

	// 実データ上のインデックス
	uint32_t denseIndex;
	// オブジェクトエンティティ
	EntityID ownerEntity;
};
