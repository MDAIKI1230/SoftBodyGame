#include "PhysicsAPI.h"

#include "SoftBodyComponentStorage.h"

bool SoftBodyComponentStorage::CanAdd(EntityID _entity)
{
	return PhysicsAPI::CanAddBody(_entity);
}

void SoftBodyComponentStorage::OnRemoving(EntityID _entity, const SoftBodyComponent& _component)
{
	PhysicsAPI::DestroyBody(_component.GetID());
}
