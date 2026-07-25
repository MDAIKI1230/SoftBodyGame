#pragma once

#include "StorageAccessorsMacros.h"

#include "ColliderSlot.h"
#include "EntityID.h"

#include "AABBBroadPhaseColliderStorage.h"
#include "SphereColliderStorage.h"
#include "BoxColliderStorage.h"

class ColliderStorage
{
    // 球コライダー
    MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ColliderID, ColliderID, SphereColliderID, sphereStorage, ID);
    MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, float, SphereColliderRadius, sphereStorage, Radius);
    // ボックスコライダー
    MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ColliderID, ColliderID, BoxColliderID, boxStorage, ID);
    MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, Vector3, BoxColliderScale, boxStorage, Scale);
    // AABB(特別処理)
    // AABB取得関数
    const AABBBroadPhaseCollider& GetAABBBroadPhaseCollider(uint32_t _index)
    {
        return aabbStorage->GetAABB(_index);
    }
    // Diary取得関数
    AABBDiaryFlag GetAABBDiaryFlag(uint32_t _index)
    {
        return aabbStorage->GetDiary(_index);
    }
    // AABB参照返し関数
    AABBBroadPhaseCollider& EditAABBBroadPhaseCollider(uint32_t _index)
    {
        return aabbStorage->EditAABB(_index);
    }
    // Diary参照返し関数
    AABBDiaryFlag& EditAABBDiaryFlag(uint32_t _index)
    {
        return aabbStorage->EditDiary(_index);
    }
    // AABBの数
    uint32_t AABBCount()
    {
        return aabbStorage->Count();
    }
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
private:
    ColliderID GenerateColliderID(ColliderType _type, uint32_t _denseIndex, uint32_t _aabbIndex, EntityID _ownerEntity, PhysicsTransformID _transformID);
private:
    std::vector<ColliderSlot> slots;
    std::vector<size_t> freeSlots;

    // AABBストレージ
    std::unique_ptr<AABBBroadPhaseColliderStorage> aabbStorage;
    // 球Storage
    std::unique_ptr<SphereColliderStorage> sphereStorage;
    // ボックスStorage
    std::unique_ptr<BoxColliderStorage> boxStorage;

    std::unordered_map<PhysicsTransformID, std::vector<ColliderID>> transformMap;
};
