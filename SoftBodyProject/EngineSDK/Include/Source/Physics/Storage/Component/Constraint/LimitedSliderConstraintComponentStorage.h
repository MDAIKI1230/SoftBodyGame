#pragma once

#include "MultiComponentStorageBase.h"

#include "LimitedSliderConstraintComponent.h"

class LimitedSliderConstraintComponentStorage :public MultiComponentStorageBase<LimitedSliderConstraintComponent>
{
	void OnRemoving(EntityID _entity, const LimitedSliderConstraintComponent& _component) override;
};