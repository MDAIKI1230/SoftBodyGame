#pragma once

#include <stdint.h>

#include "EntityID.h"
#include "BodyID.h"

struct BodySlot
{
public:
    // コンストラクタ
    BodySlot(uint32_t _denseIndex, EntityID _ownerEntity) :
        denseIndex{ _denseIndex },
        ownerEntity{ _ownerEntity }
    {
    }

public:
    // 世代
    unsigned char generation{ 1 };
    // 生存フラグ
    bool alive{ true };

    // 実データ上のインデックス
    uint32_t denseIndex;
    // オブジェクトエンティティ
    EntityID ownerEntity;
    // TransformID
    PhysicsTransformID transformID;
};
