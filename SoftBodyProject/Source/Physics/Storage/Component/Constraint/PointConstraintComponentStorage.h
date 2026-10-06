#pragma once

#include "MultiComponentStorageBase.h"

#include "PointConstraintComponent.h"

class PointConstraintComponentStorage : public MultiComponentStorageBase<PointConstraintComponent>
{
	void OnRemoving(EntityID _entity, const PointConstraintComponent& _component) override;
};