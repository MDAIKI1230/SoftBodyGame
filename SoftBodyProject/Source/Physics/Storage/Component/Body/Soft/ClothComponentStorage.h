#pragma once

#include "SparseSetStorageBase.h"

#include "ClothComponent.h"


class ClothComponentStorage :public SparseSetStorageBase<ClothComponent>
{
private:
	bool CanAdd(EntityID& _entity) override;
};
