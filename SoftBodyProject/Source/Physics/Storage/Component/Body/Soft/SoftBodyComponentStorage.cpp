#include "PhysicsComponentAPI.h"

#include "SoftBodyComponentStorage.h"

bool SoftBodyComponentStorage::CanAdd(EntityID _entity)
{
	return PhysicsComponentAPI::CanAddBody(_entity);
}

void SoftBodyComponentStorage::OnRemoving(EntityID _entity, const SoftBodyComponent& _component)
{
	PhysicsComponentAPI::DestroyBody(_component.GetID());
}
