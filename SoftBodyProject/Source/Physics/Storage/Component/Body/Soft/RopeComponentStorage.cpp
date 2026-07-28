#include "PhysicsAPI.h"

#include "RopeComponentStorage.h"

bool RopeComponentStorage::CanAdd(EntityID _entity)
{
	return PhysicsAPI::CanAddBody(_entity);
}

void RopeComponentStorage::OnRemoving(EntityID _entity, const RopeComponent& _component)
{
	PhysicsAPI::DestroyBody(_component.GetID());
}
