#pragma once

#include "MDMath.h"
#include "StorageAccessorsMacros.h"

#include "PhysicsStorageBase.h"

#include "PhysicsTransformSlot.h"
#include "PhysicsTransformID.h"

class PhysicsTransformStorage :public PhysicsStorageBase<PhysicsTransformID, PhysicsTransformSlot>
{
	// 位置
	MD_STORAGE_READ_WRITE_COLUMN(Vector3, Position, positions);
	// 回転
	MD_STORAGE_READ_WRITE_COLUMN(Quaternion, Rotation, rotations);
	// 大きさ
	MD_STORAGE_READ_WRITE_COLUMN(Vector3, Scale, scales);
	// ローカル行列
	MD_STORAGE_READ_WRITE_COLUMN(Matrix4x4, LocalMatrix, localMatrices);
	// ワールド行列
	MD_STORAGE_READ_WRITE_COLUMN(Matrix4x4, WorldMatrix, worldMatrices);
	// 親
	MD_STORAGE_READ_WRITE_COLUMN(PhysicsTransformID, ParentID, parentIDs);
	// ID
	MD_STORAGE_READ_ONLY_COLUMN(PhysicsTransformID, ID, ids);
public:
	// コンストラクタ
	PhysicsTransformStorage() = default;

	// PhysicsTransform取得もしくはないなら作成
	PhysicsTransformID GetOrCreateTransform(EntityID _entity);

	// 破棄
	void Destroy(PhysicsTransformID _id);

	// EntityIDに対応したPhysicsTransformIDがあるか
	bool TryGet(EntityID _entity, PhysicsTransformID& _output);
private:
	std::unordered_map<EntityID, PhysicsTransformID> entityMap;
};
