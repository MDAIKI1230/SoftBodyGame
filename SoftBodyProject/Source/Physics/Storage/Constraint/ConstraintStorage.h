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
	ConstraintID CreatePointConstraint(EntityID& _entity);

	// 破棄
	void Destory(ConstraintID& _id);

	// 生存確認
	bool IsAlive(ConstraintID& _id) const;
	// 実データのIndex
	uint32_t GetDenseIndex(ConstraintID& _id) const;
	// 持ってるEntity
	EntityID GetOwnerEntity(ConstraintID& _id) const;
	// TransformID
	PhysicsTransformID GetTransformID(ConstraintID& _id) const;
public:
	std::vector<ConstraintSlot> slots;
	std::unique_ptr<PointConstraintStorage> pointConstraintStorage;
};
