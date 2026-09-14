#pragma once

#include "MDMath.h"

#include "Base/ConstraintComponentBase.h"

struct DistanceConstraintComponent :public ConstraintComponentBase
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
