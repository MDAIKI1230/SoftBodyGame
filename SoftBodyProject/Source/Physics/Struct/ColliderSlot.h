#pragma once

#include <cstdint>

#include "CollisionConstants.h"
#include "EntityID.h"
#include "BodyID.h"
#include "PhysicsTransformID.h"

struct ColliderSlot
{
public:
    // コンストラクタ
    ColliderSlot(ColliderType _type, uint32_t _denseIndex, EntityID _ownerEntity, PhysicsTransformID _transformID) :
        type{ _type },
        denseIndex{ _denseIndex },
        ownerEntity{ _ownerEntity },
        transformID{ _transformID }
    {
    }

public:
    // 世代
    unsigned char generation{ 1 };
    // 生存フラグ
    bool alive{ true };

    ColliderType type;
    // 実データ上のインデックス
    uint32_t denseIndex;
    // オブジェクトエンティティ
    EntityID ownerEntity;
    // ボディ
    BodyID bodyID;
    // Transform
    PhysicsTransformID transformID;
};
