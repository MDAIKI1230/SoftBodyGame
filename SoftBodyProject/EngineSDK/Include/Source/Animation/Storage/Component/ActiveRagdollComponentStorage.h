#pragma once

#include "UniqueComponentStorageBase.h"

#include "ActiveRagdollComponent.h"

class ActiveRagdollComponentStorage :public UniqueComponentStorageBase<ActiveRagdollComponent>
{
private:
	void OnRemoving(EntityID _entity, const ActiveRagdollComponent& _component) override;
};