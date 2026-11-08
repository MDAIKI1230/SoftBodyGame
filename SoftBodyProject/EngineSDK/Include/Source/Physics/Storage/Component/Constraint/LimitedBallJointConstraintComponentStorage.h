#pragma once

#include "MultiComponentStorageBase.h"

#include "LimitedBallJointConstraintComponent.h"

class LimitedBallJointConstraintComponentStorage :public MultiComponentStorageBase<LimitedBallJointConstraintComponent>
{
	void OnRemoving(EntityID _entity, const LimitedBallJointConstraintComponent& _component) override;
};