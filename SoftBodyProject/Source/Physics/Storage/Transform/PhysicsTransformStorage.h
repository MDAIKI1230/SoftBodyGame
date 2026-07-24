#pragma once

#include <vector>
#include <unordered_map>

#include "MDMath.h"
#include "StorageAccessorsMacros.h"

#include "PhysicsTransformSlot.h"
#include "PhysicsTransformID.h"

class PhysicsTransformStorage
{
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
public:
	std::vector<PhysicsTransformSlot> slots;
	std::vector<uint32_t> freeSlots;

	// 位置
	MD_STORAGE_READ_WRITE_COLUMN(Vector3, Position, positions);
public:
	// 回転
	std::vector<Quaternion> rotation;
	// 大きさ
	std::vector<Vector3> scale;

	// ローカル行列
	std::vector<Matrix4x4> localMatrix;
	// ワールド行列
	std::vector<Matrix4x4> worldMatrix;

	// 親
	std::vector<PhysicsTransformID> parentID;

	// ID
	std::vector<PhysicsTransformID> id;
private:
	PhysicsTransformID GeneratePhysicsTransformID(size_t _denseIndex, EntityID _ownerEntity);
private:
	std::unordered_map<EntityID, PhysicsTransformID> entityMap;
};
