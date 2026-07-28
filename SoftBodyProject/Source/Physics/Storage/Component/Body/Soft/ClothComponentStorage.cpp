#include "PhysicsAPI.h"

#include "ClothComponentStorage.h"

bool ClothComponentStorage::CanAdd(EntityID _entity)
{
	return PhysicsAPI::CanAddBody(_entity);
}

void ClothComponentStorage::OnRemoving(EntityID _entity, const ClothComponent& _component)
{
	PhysicsAPI::DestroyBody(_component.GetID());
}
