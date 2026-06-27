#pragma once

#include <vector>
#include <memory>

#include "ColliderSlot.h"
#include "EntityID.h"

#include "AABBBroadPhaseColliderStorage.h"
#include "SphereColliderStorage.h"
#include "BoxColliderStorage.h"

class ColliderStorage
{
public:
    // コンストラクタ
    ColliderStorage();

    // 球作成
    ColliderID CreateSphere(EntityID _entity, float _radius);
    // Box作成
    ColliderID CreateBox(EntityID _entity, const Vector3& _scale);

    // 破棄
    void Destroy(ColliderID _id);

    // 生存確認
    bool IsAlive(ColliderID _id) const;
    // タイプ取得
    ColliderType GetType(ColliderID _id) const;
    // 実データのインデックス
    size_t GetDenseIndex(ColliderID _id) const;
    // 持ってるEntity
    EntityID GetOwnerEntity(ColliderID _id) const;
public:
    std::vector<ColliderSlot> slots;
    std::vector<size_t> freeSlots;

    // AABBストレージ
    std::unique_ptr<AABBBroadPhaseColliderStorage> aabbStorage;
    // 球Storage
    std::unique_ptr<SphereColliderStorage> sphereStorage;
    // 箱Storage
    std::unique_ptr<BoxColliderStorage> boxStorage;
private:
    ColliderID GenerateColliderID(ColliderType _type, size_t _denseIndex, EntityID _ownerEntity);
};
