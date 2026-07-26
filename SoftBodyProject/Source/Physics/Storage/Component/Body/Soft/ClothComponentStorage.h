#pragma once

#include "UniqueComponentStorageBase.h"

#include "ClothComponent.h"

class ClothComponentStorage :public UniqueComponentStorageBase<ClothComponent>
{
private:
	bool CanAdd(EntityID _entity) override;
};
