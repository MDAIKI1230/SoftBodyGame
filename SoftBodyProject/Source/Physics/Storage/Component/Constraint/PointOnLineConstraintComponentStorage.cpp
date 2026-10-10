#include "PhysicsComponentAPI.h"

#include "PointOnLineConstraintComponentStorage.h"

void PointOnLineConstraintComponentStorage::OnRemoving(EntityID _entity, const PointOnLineConstraintComponent& _component)
{
	PhysicsComponentAPI::DestroyConstraint(_component.id);
}
