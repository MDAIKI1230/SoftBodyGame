#pragma once

#include "UniqueComponentStorageBase.h"

#include "LimitedBallJointConstraintComponent.h"

class LimitedBallJointConstraintComponentStorage :public UniqueComponentStorageBase<LimitedBallJointConstraintComponent>
{
	void OnRemoving(EntityID _entity, const LimitedBallJointConstraintComponent& _component) override;
};