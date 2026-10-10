#pragma once

#include "UniqueComponentStorageBase.h"

#include "LimitedSliderConstraintComponent.h"

class LimitedSliderConstraintComponentStorage :public UniqueComponentStorageBase<LimitedSliderConstraintComponent>
{
	void OnRemoving(EntityID _entity, const LimitedSliderConstraintComponent& _component) override;
};
