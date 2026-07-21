#pragma once

#include "WorldStorage.h"
#include "PhysicsTransformStorage.h"

class SynchronizationSystem
{
public:
	void Sync(WorldStorage* _worldStorage, PhysicsTransformStorage* _physicsTransformStorage);
};
