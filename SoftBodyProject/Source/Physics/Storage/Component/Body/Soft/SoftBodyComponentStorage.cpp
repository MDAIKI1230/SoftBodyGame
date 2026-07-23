#include "PhysicsAPI.h"

#include "SoftBodyComponentStorage.h"

bool SoftBodyComponentStorage::CanAdd(EntityID& _entity)
{
	return PhysicsAPI::CanAddBody(_entity);
}
