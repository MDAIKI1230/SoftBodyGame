#pragma once

#include "StorageAccessorsMacros.h"
#include "AssertMacros.h"

#include "PhysicsStorageBase.h"

#include "ColliderSlot.h"
#include "ColliderID.h"
#include "EntityID.h"

#include "AABBBroadPhaseColliderStorage.h"
#include "SphereColliderStorage.h"
#include "BoxColliderStorage.h"
#include "CapsuleColliderStorage.h"

class ColliderStorage:public PhysicsStorageBase<ColliderID, ColliderSlot>
{
    // 球コライダー
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ColliderID, ColliderID, SphereColliderID, sphereStorage, ID)
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, float, SphereColliderRadius, sphereStorage, Radius)
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, Vector3, SphereColliderOffsetPosition, sphereStorage, OffsetPosition)
    // ボックスコライダー
    MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ColliderID, ColliderID, BoxColliderID, boxStorage, ID)
    MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, Vector3, BoxColliderSize, boxStorage, Size)
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, Vector3, BoxColliderOffsetPosition, boxStorage, OffsetPosition)
    // カプセルコライダー
    MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ColliderID, ColliderID, CapsuleColliderID, capsuleStorage, ID);
    MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, float, CapsuleColliderHeight, capsuleStorage, Height);
    MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, float, CapsuleColliderRadius, capsuleStorage, Radius);
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, Vector3, CapsuleColliderOffsetPosition, capsuleStorage, OffsetPosition)
	// AABB(特別処理)
	// AABB取得関数
	const AABBBroadPhaseCollider& GetAABBBroadPhaseCollider(uint32_t _index);
    // Diary取得関数
	AABBDiaryFlag GetAABBDiaryFlag(uint32_t _index);
    // AABB参照返し関数
	AABBBroadPhaseCollider& EditAABBBroadPhaseCollider(uint32_t _index);
    // Diary参照返し関数
	AABBDiaryFlag& EditAABBDiaryFlag(uint32_t _index);
    // AABBの数
	uint32_t AABBCount();
	// Filter取得
	const CollisionFilter& GetFilter(ColliderID _id);
	// Filter編集
	CollisionFilter& EditFilter(ColliderID _id);

	// スケール対応済み球の半径取得
	float GetScaledSphereRadius(ColliderID _id, const Vector3& _scale);
	// スケール対応済みカプセルの半径取得
	float GetScaledCapsuleRadius(ColliderID _id, const Vector3& _scale);
	// スケール対応済みカプセルの高さ取得
	float GetScaledCapsuleHeight(ColliderID _id, const Vector3& _scale);
	// スケール対応済みボックスのサイズ取得
	Vector3 GetScaledBoxSize(ColliderID _id, const Vector3& _scale);

	// スケール対応済み球オフセットの取得
	Vector3 GetScaledSphereOffset(ColliderID _id, const Vector3& _scale);
	// スケール対応済みカプセルオフセットの取得
	Vector3 GetScaledCapsuleOffset(ColliderID _id, const Vector3& _scale);
	// スケール対応済みボックスオフセットの取得
	Vector3 GetScaledBoxOffset(ColliderID _id, const Vector3& _scale);

	// 形状の判断がつかへん場合のオフセット取得
	Vector3 GetScaledOffset(ColliderID _id, const Vector3& _scale);
public:
    // コンストラクタ
    ColliderStorage();

    // 球作成
    ColliderID CreateSphere(EntityID _entity, PhysicsTransformID _transformID, float _radius);
    // ボックス作成
    ColliderID CreateBox(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _scale);
    // カプセル作成
    ColliderID CreateCapsule(EntityID _entity, PhysicsTransformID _transformID, float _height, float _radius);

    // 破棄
    void Destroy(ColliderID _id);
  
    // タイプ取得
    ColliderType GetType(ColliderID _id) const;

    // AABBのインデックス
    uint32_t GetAABBIndex(ColliderID _id) const;
    // TransformID
    PhysicsTransformID GetTransformID(ColliderID _id) const;

    // TransformIDから対応したCollider取得
    std::vector<ColliderID>& GetColliderIDFromTransformID(PhysicsTransformID _id);
private:
    // AABBストレージ
    std::unique_ptr<AABBBroadPhaseColliderStorage> aabbStorage;
    // 球Storage
    std::unique_ptr<SphereColliderStorage> sphereStorage;
    // ボックスStorage
    std::unique_ptr<BoxColliderStorage> boxStorage;
    // カプセルStorage
    std::unique_ptr<CapsuleColliderStorage> capsuleStorage;

    std::unordered_map<PhysicsTransformID, std::vector<ColliderID>> transformMap;
};