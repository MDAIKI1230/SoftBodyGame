#pragma once

#include "MultiComponentStorageBase.h"

#include "AngleLimitHingeConstraintComponent.h"

class AngleLimitHingeConstraintComponentStorage :public MultiComponentStorageBase<AngleLimitHingeConstraintComponent>
{
	void OnRemoving(EntityID _entity, const AngleLimitHingeConstraintComponent& _component) override;
};