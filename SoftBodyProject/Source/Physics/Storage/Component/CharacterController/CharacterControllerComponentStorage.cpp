#include "PhysicsComponentAPI.h"

#include "CharacterControllerComponentStorage.h"

bool CharacterControllerComponentStorage::CanAdd(EntityID _id)
{
	return PhysicsComponentAPI::CanAddCharacterController(_id);
}
void CharacterControllerComponentStorage::OnRemoving(EntityID _entity, const CharacterControllerComponent& _component)
{
	PhysicsComponentAPI::DestroyCharacterController(_component.GetID());
}
