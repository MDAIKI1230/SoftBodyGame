#pragma once

#include "UniqueComponentStorageBase.h"

#include "RopeComponent.h"


class RopeComponentStorage:public UniqueComponentStorageBase<RopeComponent>
{
private:
	bool CanAdd(EntityID _entity) override;
};
