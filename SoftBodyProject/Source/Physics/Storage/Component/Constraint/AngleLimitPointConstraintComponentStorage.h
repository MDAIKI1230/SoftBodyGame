#pragma once

#include "UniqueComponentStorageBase.h"

#include "AngleLimitPointConstraintComponent.h"

class AngleLimitPointConstraintComponentStorage :public UniqueComponentStorageBase<AngleLimitPointConstraintComponent>
{
	void OnRemoving(EntityID _entity, const AngleLimitPointConstraintComponent& _component) override;
};