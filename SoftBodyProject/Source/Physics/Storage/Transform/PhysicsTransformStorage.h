#pragma once

#include "MDMath.h"
#include "StorageAccessorsMacros.h"

#include "PhysicsTransformSlot.h"
#include "PhysicsTransformID.h"

class PhysicsTransformStorage
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

	// 生存確認
	bool IsAlive(PhysicsTransformID _id) const;
	// 実データのインデックス
	uint32_t GetDenseIndex(PhysicsTransformID _id) const;
	// 持ってるEntity
	EntityID GetOwnerEntity(PhysicsTransformID _id) const;

	// EntityIDに対応したPhysicsTransformIDがあるか
	bool TryGet(EntityID _entity,PhysicsTransformID& _output);
private:
	PhysicsTransformID GeneratePhysicsTransformID(size_t _denseIndex, EntityID _ownerEntity);
private:
	std::unordered_map<EntityID, PhysicsTransformID> entityMap;
	std::vector<PhysicsTransformSlot> slots;
	std::vector<uint32_t> freeSlots;
};
