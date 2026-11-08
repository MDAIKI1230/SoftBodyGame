#include "PhysicsComponentAPI.h"

#include "LimitedSliderConstraintComponentStorage.h"

void LimitedSliderConstraintComponentStorage::OnRemoving(EntityID _entity, const LimitedSliderConstraintComponent& _component)
{
	PhysicsComponentAPI::DestroyConstraint(_component.id);
}
