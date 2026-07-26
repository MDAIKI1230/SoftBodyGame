#pragma once

#include "UniqueComponentStorageBase.h"

#include "SoftBodyComponent.h"

class SoftBodyComponentStorage :public UniqueComponentStorageBase<SoftBodyComponent>
{
private:
	bool CanAdd(EntityID _entity) override;
};
