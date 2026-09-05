#include "PhysicsComponentAPI.h"

#include "CapsuleColliderComponentStorage.h"

void CapsuleColliderComponentStorage::OnRemoving(EntityID _entity, const CapsuleColliderComponent& _component)
{
	PhysicsComponentAPI::DestroyCollider(_component.GetID());
}
