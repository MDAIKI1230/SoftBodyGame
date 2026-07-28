#pragma once

#include "UniqueComponentStorageBase.h"

#include "RopeComponent.h"


class RopeComponentStorage:public UniqueComponentStorageBase<RopeComponent>
{
private:
	bool CanAdd(EntityID _entity) override;
	void OnRemoving(EntityID _entity, const RopeComponent& _component) override;
};
