#pragma once

#include "UniqueComponentStorageBase.h"

#include "HingeConstraintComponent.h"

class HingeConstraintComponentStorage :public UniqueComponentStorageBase<HingeConstraintComponent>
{
	void OnRemoving(EntityID _entity, const HingeConstraintComponent& _component) override;
};