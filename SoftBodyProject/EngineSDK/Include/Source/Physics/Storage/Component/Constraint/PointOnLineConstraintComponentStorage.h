#pragma once

#include "MultiComponentStorageBase.h"

#include "PointOnLineConstraintComponent.h"

class PointOnLineConstraintComponentStorage :public MultiComponentStorageBase<PointOnLineConstraintComponent>
{
	void OnRemoving(EntityID _entity, const PointOnLineConstraintComponent& _component) override;
};