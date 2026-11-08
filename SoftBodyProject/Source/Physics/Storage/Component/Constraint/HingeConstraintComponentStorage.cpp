#include "PhysicsComponentAPI.h"

#include "HingeConstraintComponentStorage.h"

void HingeConstraintComponentStorage::OnRemoving(EntityID _entity, const HingeConstraintComponent& _component)
{
	PhysicsComponentAPI::DestroyConstraint(_component.id);
}