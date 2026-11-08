#include "PhysicsComponentAPI.h"

#include "AngleLimitHingeConstraintComponentStorage.h"

void AngleLimitHingeConstraintComponentStorage::OnRemoving(EntityID _entity, const AngleLimitHingeConstraintComponent& _component)
{
	PhysicsComponentAPI::DestroyConstraint(_component.GetID());
}