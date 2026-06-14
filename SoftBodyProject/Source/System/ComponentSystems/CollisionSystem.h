#pragma once

#include "FixedUpdateSystem.h"

class CollisionSystem :public FixedUpdateSystem
{
	void FixedUpdate(IWorld* world) override;
};