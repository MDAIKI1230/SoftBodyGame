#include "PhysicsComponentAPI.h"

#include "LimitedPointOnLineConstraintComponentStorage.h"

void LimitedPointOnLineConstraintComponentStorage::OnRemoving(EntityID _entity, const LimitedPointOnLineConstraintComponent& _component)
{
	PhysicsComponentAPI::DestroyConstraint(_component.id);
}
