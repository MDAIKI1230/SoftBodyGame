#pragma once

#include "UniqueComponentStorageBase.h"

#include "LimitedPointOnLineConstraintComponent.h"

class LimitedPointOnLineConstraintComponentStorage :public UniqueComponentStorageBase<LimitedPointOnLineConstraintComponent>
{
	void OnRemoving(EntityID _entity, const LimitedPointOnLineConstraintComponent& _component) override;
};
