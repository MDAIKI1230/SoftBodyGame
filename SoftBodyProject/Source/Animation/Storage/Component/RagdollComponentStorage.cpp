#include "AnimationComponentAPI.h"

#include "RagdollComponentStorage.h"

void RagdollComponentStorage::OnRemoving(EntityID _entity, const RagdollComponent& _component)
{
	AnimationComponentAPI::DestroyRagdoll(_component.id);
}
