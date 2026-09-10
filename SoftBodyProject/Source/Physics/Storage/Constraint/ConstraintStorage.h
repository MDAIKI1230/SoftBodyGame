#pragma once

#include "StorageAccessorsMacros.h"

#include "ConstraintID.h"
#include "ConstraintSlot.h"

#include "PhysicsStorageBase.h"

#include "PointConstraintStorage.h"
#include "DistanceConstraintStorage.h"

class ConstraintStorage :public PhysicsStorageBase<ConstraintID, ConstraintSlot>
{
	// 点拘束
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ConstraintID, ConstraintID, PointConstraintID, pointConstraintStorage, ID);
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ConstraintID, PointConstraint, PointConstraint, pointConstraintStorage, Constraint);
	// 距離拘束
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(ConstraintID, ConstraintID, DistanceConstraintID, distanceConstraintStorage, ID);
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(ConstraintID, DistanceConstraint, DistanceConstraint, distanceConstraintStorage, Constraint);
public:
	// コンストラクタ
	ConstraintStorage();

	// 点拘束作成関数
	ConstraintID CreatePointConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset);
	// 距離拘束作成関数
	ConstraintID CreateDistanceConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, float _distance);

	// 破棄
	void Destory(ConstraintID _id);

	// 種類取得
	ConstraintType GetType(ConstraintID _id) const;
	// TransformID
	PhysicsTransformID GetTransformID(ConstraintID _id) const;

	// TransformIDから対応した拘束取得
	bool TryGetConstraintIDFromTransformID(PhysicsTransformID _id, ConstraintID& _output);
private:
	// --- 各種拘束ストレージ ---

	std::unique_ptr<PointConstraintStorage> pointConstraintStorage;
	std::unique_ptr<DistanceConstraintStorage> distanceConstraintStorage;

	// PhysicsTransformIDとの対応表
	std::unordered_map<PhysicsTransformID, ConstraintID> transformMap;
};
