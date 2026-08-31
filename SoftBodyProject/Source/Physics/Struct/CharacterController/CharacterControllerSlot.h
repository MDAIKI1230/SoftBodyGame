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
		transformID{ _transformID },
		colliderID{ _colliderID }
	{
	}

private:
	// 実データ上のインデックス
	uint32_t denseIndex;
	// オブジェクトエンティティ
	EntityID ownerEntity;
	// Transform
	PhysicsTransformID transformID;
	// Collider
	ColliderID colliderID;
};
