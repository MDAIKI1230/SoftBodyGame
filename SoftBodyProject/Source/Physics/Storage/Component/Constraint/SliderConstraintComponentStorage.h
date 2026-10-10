#pragma once

#include "UniqueComponentStorageBase.h"

#include "SliderConstraintComponent.h"

class SliderConstraintComponentStorage :public UniqueComponentStorageBase<SliderConstraintComponent>
{
	void OnRemoving(EntityID _entity, const SliderConstraintComponent& _component) override;
};
