#pragma once

#include "UniqueComponentStorageBase.h"

#include "AnimationComponent.h"

class AnimationComponentStorage :public UniqueComponentStorageBase<AnimationComponent>
{
private:
	void OnRemoving(EntityID _entity, const AnimationComponent& _component) override;
};