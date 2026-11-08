#pragma once

#include "MultiComponentStorageBase.h"

#include "CapsuleColliderComponent.h"

class CapsuleColliderComponentStorage :public MultiComponentStorageBase<CapsuleColliderComponent>
{
	void OnRemoving(EntityID _entity, const CapsuleColliderComponent& _component) override;
};
