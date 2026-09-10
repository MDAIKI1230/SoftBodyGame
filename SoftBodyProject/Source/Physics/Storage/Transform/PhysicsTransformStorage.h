#pragma once

#include "MDMath.h"
#include "StorageAccessorsMacros.h"

#include "SyncConstants.h"

#include "DataStorageBase.h"

#include "PhysicsTransformID.h"

class PhysicsTransformStorage :public DataStorageBase<PhysicsTransformID>
{
	// 位置
	MD_STORAGE_ID_READ_WRITE_COLUMN(PhysicsTransformID,Vector3, Position, positions)
	// 回転
	MD_STORAGE_ID_READ_WRITE_COLUMN(PhysicsTransformID, Quaternion, Rotation, rotations)
	// 大きさ
	MD_STORAGE_ID_READ_WRITE_COLUMN(PhysicsTransformID, Vector3, Scale, scales)
	// ローカル行列
	MD_STORAGE_ID_READ_WRITE_COLUMN(PhysicsTransformID, Matrix4x4, LocalMatrix, localMatrices)
	// ワールド行列
	MD_STORAGE_ID_READ_WRITE_COLUMN(PhysicsTransformID, Matrix4x4, WorldMatrix, worldMatrices)
	// 親
	MD_STORAGE_ID_READ_WRITE_COLUMN(PhysicsTransformID, PhysicsTransformID, ParentID, parentIDs)
	// ECSとの関係
	MD_STORAGE_ID_READ_ONLY_COLUMN(PhysicsTransformID, PhysicsTransformSyncPolicy, SyncPolicy, syncPolicies)
	// 使用エンティティ
	MD_STORAGE_ID_READ_ONLY_COLUMN(PhysicsTransformID, EntityID, OwnerEntity, ownerEntities)
	// ID
	MD_STORAGE_ID_READ_ONLY_COLUMN(PhysicsTransformID, PhysicsTransformID, ID, ids)
public:
	// コンストラクタ
	PhysicsTransformStorage() = default;

	// PhysicsTransform取得もしくはないなら作成
	PhysicsTransformID GetOrCreateTransform(EntityID _entity);
	// 内部用のTransform作成
	PhysicsTransformID CreateInternalTransform(EntityID _entity, const Vector3& _position, const Quaternion& _rotation, const Vector3& _scale);

	// 破棄
	void Destroy(PhysicsTransformID _id);

	// EntityIDに対応したPhysicsTransformIDがあるか
	bool TryGet(EntityID _entity, PhysicsTransformID& _output);
private:
	std::unordered_map<EntityID, PhysicsTransformID> entityMap;
};
