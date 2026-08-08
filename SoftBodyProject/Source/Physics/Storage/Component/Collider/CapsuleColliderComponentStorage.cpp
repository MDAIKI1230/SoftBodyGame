#include "PhysicsAPI.h"

#include "CapsuleColliderComponentStorage.h"

void CapsuleColliderComponentStorage::OnRemoving(EntityID _entity, const CapsuleColliderComponent& _component)
{
	PhysicsAPI::DestroyCollider(_component.GetID());
}
