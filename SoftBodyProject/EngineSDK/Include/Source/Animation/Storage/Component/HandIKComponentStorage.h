#pragma once

#include "UniqueComponentStorageBase.h"

#include "HandIKComponent.h"

class HandIKComponentStorage :public UniqueComponentStorageBase<HandIKComponent>
{
private:
	void OnRemoving(EntityID _entity, const HandIKComponent& _component) override;
};