#pragma once

#include "UniqueComponentStorageBase.h"

#include "PointConstraintComponent.h"

class PointConstraintComponentStorage :public UniqueComponentStorageBase<PointConstraintComponent>
{
	void OnRemoving(EntityID _entity, const  PointConstraintComponent& _component) override;
};

