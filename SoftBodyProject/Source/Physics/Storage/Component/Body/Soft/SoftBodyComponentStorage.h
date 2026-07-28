#pragma once

#include "UniqueComponentStorageBase.h"

#include "SoftBodyComponent.h"

class SoftBodyComponentStorage :public UniqueComponentStorageBase<SoftBodyComponent>
{
private:
	bool CanAdd(EntityID _entity) override;
	void OnRemoving(EntityID _entity, const SoftBodyComponent& _component) override;
};
