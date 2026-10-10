#pragma once

#include "UniqueComponentStorageBase.h"

#include "PointOnLineConstraintComponent.h"

class PointOnLineConstraintComponentStorage :public UniqueComponentStorageBase<PointOnLineConstraintComponent>
{
	void OnRemoving(EntityID _entity, const PointOnLineConstraintComponent& _component) override;
};
