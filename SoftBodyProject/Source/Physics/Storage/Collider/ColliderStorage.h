#pragma once

#include <vector>
#include <memory>
#include <unordered_map>

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
    ColliderID CreateSphere(EntityID _entity, PhysicsTransformID _transformID, float _radius);
    // Box作成
    ColliderID CreateBox(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _scale);

    // 破棄
    void Destroy(ColliderID _id);

    // 生存確認
    bool IsAlive(ColliderID _id) const;
    // タイプ取得
    ColliderType GetType(ColliderID _id) const;
    // 実データのインデックス
    uint32_t GetDenseIndex(ColliderID _id) const;
    // AABBのインデックス
    uint32_t GetAABBIndex(ColliderID _id) const;
    // 持ってるEntity
    EntityID GetOwnerEntity(ColliderID _id) const;
    // TransformID
    PhysicsTransformID GetTransformID(ColliderID _id) const;

    // TransformIDから対応したCollider取得
    std::vector<ColliderID>& GetColliderIDFromTransformID(PhysicsTransformID _id);
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
    ColliderID GenerateColliderID(ColliderType _type, uint32_t _denseIndex, uint32_t _aabbIndex, EntityID _ownerEntity, PhysicsTransformID _transformID);
private:
    std::unordered_map<PhysicsTransformID, std::vector<ColliderID>> transformMap;
};
