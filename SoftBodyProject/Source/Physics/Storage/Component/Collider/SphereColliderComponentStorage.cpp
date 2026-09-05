#include "PhysicsComponentAPI.h"

#include "SphereColliderComponentStorage.h"

void SphereColliderComponentStorage::OnRemoving(EntityID _entity, const SphereColliderComponent& _component)
{
	PhysicsComponentAPI::DestroyCollider(_component.GetID());
}
