#include "PhysicsComponentAPI.h"

#include "ClothComponentStorage.h"

bool ClothComponentStorage::CanAdd(EntityID _entity)
{
	return PhysicsComponentAPI::CanAddBody(_entity);
}

void ClothComponentStorage::OnRemoving(EntityID _entity, const ClothComponent& _component)
{
	PhysicsComponentAPI::DestroyBody(_component.GetID());
}
