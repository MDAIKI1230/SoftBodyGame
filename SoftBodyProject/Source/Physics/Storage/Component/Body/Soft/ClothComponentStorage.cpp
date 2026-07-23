#include "PhysicsAPI.h"

#include "ClothComponentStorage.h"

bool ClothComponentStorage::CanAdd(EntityID& _entity)
{
	return PhysicsAPI::CanAddBody(_entity);
}
