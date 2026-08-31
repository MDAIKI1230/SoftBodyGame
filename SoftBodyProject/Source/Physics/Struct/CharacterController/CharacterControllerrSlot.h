#pragma once

#include <cstdint>

#include "EntityID.h"
#include "PhysicsTransformID.h"
#include "ColliderID.h"

struct CharacterControllerSlot
{
public:
	CharacterControllerSlot(uint32_t _denseIndex, EntityID _ownerEntity, PhysicsTransformID _transformID, ColliderID _colliderID) :
		denseIndex{ _denseIndex },
		ownerEntity{ _ownerEntity },
		transformID{ _transformID }
	{
	}

private:
	// 世代
	uint32_t generation{ 1 };
	// 生存フラグ
	bool alive{ true };

	// 実データ上のインデックス
	uint32_t denseIndex;
	// オブジェクトエンティティ
	EntityID ownerEntity;
	// Transform
	PhysicsTransformID transformID;
};
