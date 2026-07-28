#include "PhysicsAPI.h"

#include "SphereColliderComponentStorage.h"

void SphereColliderComponentStorage::OnRemoving(EntityID _entity, const SphereColliderComponent& _component)
{
	PhysicsAPI::DestroyCollider(_component.GetID());
}
