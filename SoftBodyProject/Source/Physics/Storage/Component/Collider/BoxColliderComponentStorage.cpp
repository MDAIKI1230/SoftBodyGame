#include "PhysicsComponentAPI.h"

#include "BoxColliderComponentStorage.h"

void BoxColliderComponentStorage::OnRemoving(EntityID _entity, const BoxColliderComponent& _component)
{
	PhysicsComponentAPI::DestroyCollider(_component.GetID());
}
