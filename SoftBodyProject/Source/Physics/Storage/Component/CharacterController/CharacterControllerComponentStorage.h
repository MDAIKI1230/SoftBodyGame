#pragma once

#include "UniqueComponentStorageBase.h"

#include "CharacterControllerComponent.h"

class CharacterControllerComponentStorage :public UniqueComponentStorageBase<CharacterControllerComponent>
{
	bool CanAdd(EntityID _id) override;
	void OnRemoving(EntityID _entity, const CharacterControllerComponent& _component) override;
};
