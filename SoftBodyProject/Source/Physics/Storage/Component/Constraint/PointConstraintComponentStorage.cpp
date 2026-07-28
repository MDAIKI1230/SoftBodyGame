#include "PhysicsAPI.h"

#include "PointConstraintComponentStorage.h"

void PointConstraintComponentStorage::OnRemoving(EntityID _entity, const  PointConstraintComponent& _component)
{
	PhysicsAPI::DestroyConstraint(_component.GetID());
}
