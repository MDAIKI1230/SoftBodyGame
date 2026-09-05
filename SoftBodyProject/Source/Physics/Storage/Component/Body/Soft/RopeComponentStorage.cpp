#include "PhysicsComponentAPI.h"

#include "RopeComponentStorage.h"

bool RopeComponentStorage::CanAdd(EntityID _entity)
{
	return PhysicsComponentAPI::CanAddBody(_entity);
}

void RopeComponentStorage::OnRemoving(EntityID _entity, const RopeComponent& _component)
{
	PhysicsComponentAPI::DestroyBody(_component.GetID());
}
