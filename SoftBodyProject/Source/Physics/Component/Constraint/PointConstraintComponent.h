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
	// コンストラクタ
	PointConstraintComponent(EntityID _entity, int _handle,Vector3 _localOffset);

	// 対応点追加
	void AddEndPoint(const EntityID& _entityID, const Vector3& _localOffset);
private:
	ConstraintID id;
};
