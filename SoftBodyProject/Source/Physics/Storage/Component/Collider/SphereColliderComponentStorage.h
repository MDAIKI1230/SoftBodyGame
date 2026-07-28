#pragma once

#include "MultiComponentStorageBase.h"

#include "SphereColliderComponent.h"

class SphereColliderComponentStorage : public MultiComponentStorageBase<SphereColliderComponent>
{
	void OnRemoving(EntityID _entity, const SphereColliderComponent& _component) override;
};

