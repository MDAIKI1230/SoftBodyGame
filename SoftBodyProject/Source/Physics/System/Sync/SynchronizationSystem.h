#pragma once

#include "WorldStorage.h"
#include "RigidBodyStorage.h"

class SynchronizationSystem
{
public:
	void Sync(WorldStorage* _worldStorage, RigidBodyStorage* _bodyStorage);
};
