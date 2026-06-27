#pragma once

#include <cstdint>

#include "CollisionConstants.h"
#include "EntityID.h"

struct ColliderSlot
{
public:
    // コンストラクタ
    ColliderSlot(ColliderType _type, size_t _denseIndex, EntityID _ownerEntity) :
        type{ _type },
        denseIndex{ _denseIndex },
        ownerEntity{ _ownerEntity }
    {
    }

public:
    // 世代
    unsigned char generation{ 1 };
    // 生存フラグ
    bool alive{ true };

    ColliderType type;
    // 実データ上のインデックス
    size_t denseIndex;
    // オブジェクトエンティティ
    EntityID ownerEntity;
};
