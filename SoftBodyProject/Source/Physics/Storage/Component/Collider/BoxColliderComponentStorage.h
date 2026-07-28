#pragma once

#include "MultiComponentStorageBase.h"

#include "BoxColliderComponent.h"

class BoxColliderComponentStorage:public MultiComponentStorageBase<BoxColliderComponent>
{
	void OnRemoving(EntityID _entity, const BoxColliderComponent& _component) override;
};
