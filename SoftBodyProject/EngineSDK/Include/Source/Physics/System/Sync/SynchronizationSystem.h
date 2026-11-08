#pragma once

#include "WorldStorage.h"
#include "PhysicsTransformStorage.h"

class ColliderStorage;
class BodyStorage;

class SynchronizationSystem
{
public:
	void Sync(
		WorldStorage* _worldStorage, PhysicsTransformStorage* _physicsTransformStorage,
		ColliderStorage* _colliderStorage, BodyStorage* _bodyStorage);
};