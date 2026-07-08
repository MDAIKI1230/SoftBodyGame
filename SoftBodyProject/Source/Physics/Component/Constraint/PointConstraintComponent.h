#pragma once

#include "ComponentBase.h"

#include "MDMath.h"

#include "EntityID.h"

#include "ConstraintID.h"

struct PointConstraintComponent : public ComponentBase
{
public:
	// コンストラクタ
	PointConstraintComponent(EntityID _entity, int _handle);

	// 対応点追加
	void AddEndPoint(EntityID& _entityID, Vector3& _localOffset);
private:
	ConstraintID id;
};
