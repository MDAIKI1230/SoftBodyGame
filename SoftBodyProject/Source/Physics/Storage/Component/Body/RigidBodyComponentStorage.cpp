#include "PhysicsAPI.h"

#include "RigidBodyComponentStorage.h"

bool RigidBodyComponentStorage::CanAdd(EntityID _entity)
{
	return PhysicsAPI::CanAddBody(_entity);
}

void RigidBodyComponentStorage::OnRemoving(EntityID _entity, const RigidBodyComponent& _component)
{
	PhysicsAPI::DestroyBody(_component.GetID());
}
