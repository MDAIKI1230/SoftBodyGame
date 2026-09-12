#pragma once

#include "UniqueComponentStorageBase.h"

#include "AngleLimitHingeConstraintComponent.h"

class AngleLimitHingeConstraintComponentStorage :public UniqueComponentStorageBase<AngleLimitHingeConstraintComponent>
{
	void OnRemoving(EntityID _entity, const AngleLimitHingeConstraintComponent& _component) override;
};