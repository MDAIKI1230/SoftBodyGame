#include "AnimationComponentAPI.h"

#include "HandIKComponentStorage.h"

void HandIKComponentStorage::OnRemoving(EntityID _entity, const HandIKComponent& _component)
{
	AnimationComponentAPI::DestroyIK(_component.id);
}