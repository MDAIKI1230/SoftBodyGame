#pragma once

#include "MultiComponentStorageBase.h"

#include "DistanceConstraintComponent.h"

class DistanceConstraintComponentStorage :public MultiComponentStorageBase<DistanceConstraintComponent>
{
	void OnRemoving(EntityID _entity, const DistanceConstraintComponent& _component) override;
};