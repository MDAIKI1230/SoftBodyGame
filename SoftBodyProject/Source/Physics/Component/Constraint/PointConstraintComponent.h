#pragma once

#include "ComponentBase.h"

#include "MDMath.h"

#include "EntityID.h"

#include "ConstraintID.h"

struct PointConstraintComponent : public ComponentBase
{
public:
	// コンストラクタ
	PointConstraintComponent(int _handle) :
		ComponentBase{ _handle }
	{
	}

	void AddEndPoint(EntityID& _entityID, Vector3& _localOffset);
private:
	ConstraintID id;
};
