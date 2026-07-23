#pragma once

#include "MDMath.h"

#include "EntityID.h"

#include "ConstraintID.h"

struct PointConstraintComponent
{
public:
	// コンストラクタ
	PointConstraintComponent(EntityID _entity);
	// コンストラクタ
	PointConstraintComponent(EntityID _entity,Vector3 _localOffset);

	// 対応点追加
	void AddEndPoint(const EntityID& _entityID, const Vector3& _localOffset);
private:
	ConstraintID id;
};
