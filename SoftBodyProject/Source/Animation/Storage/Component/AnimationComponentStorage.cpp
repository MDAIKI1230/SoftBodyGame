#include "AnimationComponentAPI.h"

#include "AnimationComponentStorage.h"

void AnimationComponentStorage::OnRemoving(EntityID _entity, const AnimationComponent& _component)
{
	AnimationComponentAPI::DestroyAnimation(_component.id);
}