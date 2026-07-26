#pragma once

#include "MDMath.h"

#include "UniqueComponentStorageBase.h"

#include "RigidBodyComponent.h"

class RigidBodyComponentStorage :public UniqueComponentStorageBase<RigidBodyComponent>
{
private:
	bool CanAdd(EntityID _entity) override;
};
