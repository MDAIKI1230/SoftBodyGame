#pragma once

#include "SparseSetStorageBase.h"

#include "RopeComponent.h"


class RopeComponentStorge:public SparseSetStorageBase<RopeComponent>
{
private:
	bool CanAdd(EntityID& _entity) override;
};
