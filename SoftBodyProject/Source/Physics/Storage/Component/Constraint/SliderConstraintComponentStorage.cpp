#include "PhysicsComponentAPI.h"

#include "SliderConstraintComponentStorage.h"

void SliderConstraintComponentStorage::OnRemoving(EntityID _entity, const SliderConstraintComponent& _component)
{
	PhysicsComponentAPI::DestroyConstraint(_component.id);
}
