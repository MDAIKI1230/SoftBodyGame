#pragma once

#include "MDMath.h"

#include "EndPointConstraintComponentBase.h"

struct DistanceConstraintComponent :public EndPointConstraintComponentBase
{
public:
	// コンストラクタ
	DistanceConstraintComponent(EntityID _entity);
	// コンストラクタ
	DistanceConstraintComponent(EntityID _entity, Vector3 _localOffset);
	// コンストラクタ
	DistanceConstraintComponent(EntityID _entity, float _distance);
	// コンストラクタ
	DistanceConstraintComponent(EntityID _entity, Vector3 _localOffset, float _distance);

	// 距離取得
	float GetDistance();
	// 距離変更
	void SetDistance(float _distance);
};
