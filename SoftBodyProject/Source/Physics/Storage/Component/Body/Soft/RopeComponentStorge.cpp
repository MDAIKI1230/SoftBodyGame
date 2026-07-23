#include "PhysicsAPI.h"

#include "RopeComponentStorge.h"

bool RopeComponentStorge::CanAdd(EntityID& _entity)
{
	return PhysicsAPI::CanAddBody(_entity);
}
