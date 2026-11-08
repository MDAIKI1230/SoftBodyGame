#include "PhysicsComponentAPI.h"

#include "LimitedBallJointConstraintComponentStorage.h"

void LimitedBallJointConstraintComponentStorage::OnRemoving(EntityID _entity, const LimitedBallJointConstraintComponent& _component)
{
	PhysicsComponentAPI::DestroyConstraint(_component.id);
}