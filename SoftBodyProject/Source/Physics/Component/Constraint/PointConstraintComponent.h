#pragma once

#include "MDMath.h"

#include "Base/EndPointConstraintComponentBase.h"

struct PointConstraintComponent :public EndPointConstraintComponentBase
{
public:
	// コンストラクタ
	PointConstraintComponent(EntityID _entity);
	// コンストラクタ
	PointConstraintComponent(EntityID _entity,Vector3 _localOffset);
};
