#pragma once

#include "WorldStorage.h"
#include "PhysicsTransformStorage.h"

class PhysicsCommitSystem
{
public:
	// コンストラクタ
	PhysicsCommitSystem() = default;
	// 更新
	void FixedUpdate(PhysicsTransformStorage* _physicsTransformStorage, WorldStorage* _worldStorage);
};
