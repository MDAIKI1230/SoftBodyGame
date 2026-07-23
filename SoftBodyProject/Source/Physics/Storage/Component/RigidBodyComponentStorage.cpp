#include "PhysicsAPI.h"

#include "RigidBodyComponentStorage.h"

bool RigidBodyComponentStorage::CanAdd(EntityID& _entity)
{
	return PhysicsAPI::CanAddBody(_entity);
}
