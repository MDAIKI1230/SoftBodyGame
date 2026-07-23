#pragma once

#include "MDMath.h"

#include "SparseSetStorageBase.h"

#include "RigidBodyComponent.h"

class RigidBodyComponentStorage :public SparseSetStorageBase<RigidBodyComponent>
{
private:
	bool CanAdd(EntityID& _entity) override;
};
