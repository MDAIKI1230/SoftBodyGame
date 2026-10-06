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
	// Bodyを指定して点拘束を作成
	PointConstraintComponent(EntityID _entity, const RigidBodyComponent& _body, const Vector3& _localOffset = Vector3::ZERO);
};
