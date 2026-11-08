#pragma once

#include "UniqueComponentStorageBase.h"

#include "RigidBodyComponent.h"

class RigidBodyComponentStorage :public UniqueComponentStorageBase<RigidBodyComponent>
{
private:
	bool CanAdd(EntityID _entity) override;
	void OnRemoving(EntityID _entity, const RigidBodyComponent& _component) override;
};
