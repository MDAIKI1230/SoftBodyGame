#include "PhysicsAPI.h"

#include "RopeComponentStorage.h"

bool RopeComponentStorage::CanAdd(EntityID _entity)
{
	return PhysicsAPI::CanAddBody(_entity);
}
