#pragma once

#include "MDMath.h"

#include "Base/ConstraintComponentBase.h"

struct PointConstraintComponent :public ConstraintComponentBase
{
public:
	// コンストラクタ
	PointConstraintComponent(EntityID _entity);
	// コンストラクタ
	PointConstraintComponent(EntityID _entity,Vector3 _localOffset);
};
