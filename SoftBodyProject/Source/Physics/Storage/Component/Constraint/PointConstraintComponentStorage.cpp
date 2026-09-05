#include "PhysicsComponentAPI.h"

#include "PointConstraintComponentStorage.h"

void PointConstraintComponentStorage::OnRemoving(EntityID _entity, const  PointConstraintComponent& _component)
{
	PhysicsComponentAPI::DestroyConstraint(_component.GetID());
}
