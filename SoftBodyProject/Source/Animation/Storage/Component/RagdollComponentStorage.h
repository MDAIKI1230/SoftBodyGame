#pragma once

#include "UniqueComponentStorageBase.h"

#include "RagdollComponent.h"

class RagdollComponentStorage :public UniqueComponentStorageBase<RagdollComponent>
{
private:
	void OnRemoving(EntityID _entity, const RagdollComponent& _component) override;
};
