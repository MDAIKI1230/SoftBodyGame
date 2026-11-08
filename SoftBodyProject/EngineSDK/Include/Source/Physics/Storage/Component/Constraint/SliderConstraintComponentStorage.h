#pragma once

#include "MultiComponentStorageBase.h"

#include "SliderConstraintComponent.h"

class SliderConstraintComponentStorage :public MultiComponentStorageBase<SliderConstraintComponent>
{
	void OnRemoving(EntityID _entity, const SliderConstraintComponent& _component) override;
};