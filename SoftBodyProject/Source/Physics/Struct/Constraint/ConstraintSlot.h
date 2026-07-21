#pragma once

#include <cstdint>

#include "ConstraintConstants.h"

#include "EntityID.h"
#include "PhysicsTransformID.h"

struct ConstraintSlot
{
public:
    ConstraintSlot(ConstraintType _type, uint32_t _denseIndex, EntityID _ownerEntity, PhysicsTransformID _transformID) :
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

    // 種類
    ConstraintType type;
    // 実データ上のインデックス
    uint32_t denseIndex;
    // オブジェクトエンティティ
    EntityID ownerEntity;
    // Transform
    PhysicsTransformID transformID;
};
