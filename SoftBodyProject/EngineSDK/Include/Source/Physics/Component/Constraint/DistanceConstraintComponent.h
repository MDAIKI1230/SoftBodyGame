#pragma once

#include "MDMath.h"

#include "DistanceConstraint.h"

#include "Base/ConstraintComponentBase.h"

struct DistanceConstraintComponent :public ConstraintComponentBase<DistanceConstraint, false>
{
public:
	// コンストラクタ
	DistanceConstraintComponent(EntityID _entity);
	// コンストラクタ
	DistanceConstraintComponent(EntityID _entity, Vector3 _localOffset);

	// 距離取得
	float GetDistance();
	// 距離変更
	void SetDistance(float _distance);
};
