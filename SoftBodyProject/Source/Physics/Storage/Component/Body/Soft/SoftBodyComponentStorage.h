#pragma once

#include "SparseSetStorageBase.h"

#include "SoftBodyComponent.h"

class SoftBodyComponentStorage :public SparseSetStorageBase<SoftBodyComponent>
{
private:
	bool CanAdd(EntityID _entity) override;
};
