#include "PhysicsComponentAPI.h"

#include "AngleLimitPointConstraintComponentStorage.h"

void AngleLimitPointConstraintComponentStorage::OnRemoving(EntityID _entity, const AngleLimitPointConstraintComponent& _component)
{
	PhysicsComponentAPI::DestroyConstraint(_component.id);
}