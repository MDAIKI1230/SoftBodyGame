#include "PhysicsAPI.h"

#include "DistanceConstraintComponentStorage.h"

void DistanceConstraintComponentStorage::OnRemoving(EntityID _entity, const DistanceConstraintComponent& _component)
{
	PhysicsAPI::DestroyConstraint(_component.GetID());
}
