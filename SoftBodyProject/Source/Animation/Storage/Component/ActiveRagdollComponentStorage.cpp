#include "AnimationComponentAPI.h"

#include "ActiveRagdollComponentStorage.h"

void ActiveRagdollComponentStorage::OnRemoving(EntityID _entity, const ActiveRagdollComponent& _component)
{
	AnimationComponentAPI::DestroyActiveRagdoll(_component.id);
}
