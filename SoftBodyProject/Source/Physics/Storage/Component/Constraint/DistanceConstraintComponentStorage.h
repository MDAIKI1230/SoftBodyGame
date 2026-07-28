#pragma once

#include "UniqueComponentStorageBase.h"

#include "DistanceConstraintComponent.h"

class DistanceConstraintComponentStorage :public UniqueComponentStorageBase<DistanceConstraintComponent>
{
	void OnRemoving(EntityID _entity, const DistanceConstraintComponent& _component) override;
};

