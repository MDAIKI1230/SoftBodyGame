#pragma once

#include <vector>
#include <memory>

#include "ConstraintID.h"
#include "ConstraintSlot.h"

#include "PointConstraintStorage.h"
#include "DistanceConstraintStorage.h"

class ConstraintStorage
{
public:
	// コンストラクタ
	ConstraintStorage();

	// 点拘束作成関数
	ConstraintID CreatePointConstraint(EntityID& _entity, PhysicsTransformID _transformID,const Vector3& _localOffset);
	// 距離拘束作成関数
	ConstraintID CreateDistanceConstraint(EntityID& _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, float _distance);

	// 破棄
	void Destory(ConstraintID& _id);

	// 生存確認
	bool IsAlive(ConstraintID& _id) const;
	// 種類取得
	ConstraintType GetType(ConstraintID& _id) const;
	// 実データのIndex
	uint32_t GetDenseIndex(ConstraintID& _id) const;
	// 持ってるEntity
	EntityID GetOwnerEntity(ConstraintID& _id) const;
	// TransformID
	PhysicsTransformID GetTransformID(ConstraintID& _id) const;
public:
	std::vector<ConstraintSlot> slots;
	std::vector<uint32_t> freeSlots;

	// --- 各種拘束ストレージ ---

	std::unique_ptr<PointConstraintStorage> pointConstraintStorage;
	std::unique_ptr<DistanceConstraintStorage> distanceConstraintStorage;
private:
	ConstraintID GenerateConstraintID(ConstraintType _type, uint32_t _denseIndex, EntityID _ownerEntity, PhysicsTransformID _transformID);
};
