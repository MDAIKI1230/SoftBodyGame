#include "PhysicsComponentAPI.h"

#include "DistanceConstraintComponentStorage.h"

void DistanceConstraintComponentStorage::OnRemoving(EntityID _entity, const DistanceConstraintComponent& _component)
{
	PhysicsComponentAPI::DestroyConstraint(_component.GetID());
}
