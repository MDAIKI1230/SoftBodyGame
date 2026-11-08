#pragma once

#include "MultiComponentStorageBase.h"

#include "LimitedPointOnLineConstraintComponent.h"

class LimitedPointOnLineConstraintComponentStorage :public MultiComponentStorageBase<LimitedPointOnLineConstraintComponent>
{
	void OnRemoving(EntityID _entity, const LimitedPointOnLineConstraintComponent& _component) override;
};