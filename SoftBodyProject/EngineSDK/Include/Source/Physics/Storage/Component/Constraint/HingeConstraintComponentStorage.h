#pragma once

#include "MultiComponentStorageBase.h"

#include "HingeConstraintComponent.h"

class HingeConstraintComponentStorage :public MultiComponentStorageBase<HingeConstraintComponent>
{
	void OnRemoving(EntityID _entity, const HingeConstraintComponent& _component) override;
};