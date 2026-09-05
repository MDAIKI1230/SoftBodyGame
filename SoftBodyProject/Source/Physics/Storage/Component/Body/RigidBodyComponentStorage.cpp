#include "PhysicsComponentAPI.h"

#include "RigidBodyComponentStorage.h"

bool RigidBodyComponentStorage::CanAdd(EntityID _entity)
{
	return PhysicsComponentAPI::CanAddBody(_entity);
}

void RigidBodyComponentStorage::OnRemoving(EntityID _entity, const RigidBodyComponent& _component)
{
	PhysicsComponentAPI::DestroyBody(_component.GetID());
}
