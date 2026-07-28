#include "PhysicsAPI.h"

#include "BoxColliderComponentStorage.h"

void BoxColliderComponentStorage::OnRemoving(EntityID _entity, const BoxColliderComponent& _component)
{
	PhysicsAPI::DestroyCollider(_component.GetID());
}
