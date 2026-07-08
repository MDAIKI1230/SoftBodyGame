#pragma once

#include <cstdint>

#include "EntityID.h"
#include "PhysicsTransformID.h"

struct ConstraintSlot
{
    // 世代
    unsigned char generation{ 1 };
    // 生存フラグ
    bool alive{ true };

    // 実データ上のインデックス
    uint32_t denseIndex;
    // オブジェクトエンティティ
    EntityID ownerEntity;
    // Transform
    PhysicsTransformID transformID;
};
