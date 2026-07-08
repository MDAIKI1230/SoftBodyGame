#pragma once

#include <vector>
#include <memory>

#include "ConstraintID.h"
#include "ConstraintSlot.h"

#include "PointConstraintStorage.h"

class ConstraintStorage
{
public:
	// 点拘束作成関数
	ConstraintID CreatePointConstraint(EntityID& _entity, PhysicsTransformID _transformID,const Vector3& _localOffset);

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
	std::unique_ptr<PointConstraintStorage> pointConstraintStorage;
private:
	ConstraintID GenerateConstraintID(ConstraintType _type, uint32_t _denseIndex, EntityID _ownerEntity, PhysicsTransformID _transformID);
};
