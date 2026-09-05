#pragma once

#include <stdint.h>

#include "BodyConstants.h"

#include "EntityID.h"
#include "PhysicsTransformID.h"

struct BodySlot
{
public:
    // コンストラクタ
    BodySlot(uint32_t _denseIndex, BodyType _type, EntityID _ownerEntity,PhysicsTransformID _transformID) :
        denseIndex{ _denseIndex },
        type{ _type },
        ownerEntity{ _ownerEntity },
        transformID{ _transformID }
    {
    }

public:
    // 世代
    uint32_t generation{ 1 };
    // 生存フラグ
    bool alive{ true };

    // 実データ上のインデックス
    uint32_t denseIndex;
    // ボディの種類
    BodyType type;
    // オブジェクトエンティティ
    EntityID ownerEntity;
    // TransformID
    PhysicsTransformID transformID;
};
