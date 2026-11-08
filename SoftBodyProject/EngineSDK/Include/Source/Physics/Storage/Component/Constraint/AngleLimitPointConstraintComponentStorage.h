#pragma once

#include "MultiComponentStorageBase.h"

#include "AngleLimitPointConstraintComponent.h"

class AngleLimitPointConstraintComponentStorage :public MultiComponentStorageBase<AngleLimitPointConstraintComponent>
{
	void OnRemoving(EntityID _entity, const AngleLimitPointConstraintComponent& _component) override;
};