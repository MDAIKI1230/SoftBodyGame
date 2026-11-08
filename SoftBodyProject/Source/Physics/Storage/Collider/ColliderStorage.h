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
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, Quaternion, SphereColliderOffsetRotation, sphereStorage, OffsetRotation)
    // ボックスコライダー
    MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ColliderID, ColliderID, BoxColliderID, boxStorage, ID)
    MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, Vector3, BoxColliderScale, boxStorage, Scale)
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, Vector3, BoxColliderOffsetPosition, boxStorage, OffsetPosition)
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, Quaternion, BoxColliderOffsetRotation, boxStorage, OffsetRotation)
    // カプセルコライダー
    MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ColliderID, ColliderID, CapsuleColliderID, capsuleStorage, ID);
    MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, float, CapsuleColliderHeight, capsuleStorage, Height);
    MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, float, CapsuleColliderRadius, capsuleStorage, Radius);
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, Vector3, CapsuleColliderOffsetPosition, capsuleStorage, OffsetPosition)
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ColliderID, Quaternion, CapsuleColliderOffsetRotation, capsuleStorage, OffsetRotation)
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
	// Filter取得
	const CollisionFilter& GetFilter(ColliderID _id)
	{
		return GetSlot(_id).filter;
	}
	// Filter編集
	CollisionFilter& EditFilter(ColliderID _id)
	{
		return EditSlotMember<&ColliderSlot::filter>(_id);
	}

	// 形状の判断がつかへん場合のオフセット取得
	Vector3 GetOffsetPosition(ColliderID _id)
	{
		switch (GetType(_id))
		{
		case ColliderType::SPHERE:
			return GetSphereColliderOffsetPosition(_id);
			break;
		case ColliderType::BOX:
			return GetBoxColliderOffsetPosition(_id);
			break;
		case ColliderType::CAPSULE:
			return GetCapsuleColliderOffsetPosition(_id);
			break;
		default:
			MD_UNREACHABLE("おいおいこれなんや、このコライダーわけわからへん。");
			break;
		}
	}
	Quaternion GetOffsetRotation(ColliderID _id)
	{
		switch (GetType(_id))
		{
		case ColliderType::SPHERE:
			return GetSphereColliderOffsetRotation(_id);
			break;
		case ColliderType::BOX:
			return GetBoxColliderOffsetRotation(_id);
			break;
		case ColliderType::CAPSULE:
			return GetCapsuleColliderOffsetRotation(_id);
			break;
		default:
			MD_UNREACHABLE("おいおいこれなんや、このコライダーわけわからへん。");
			break;
		}
	}
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
