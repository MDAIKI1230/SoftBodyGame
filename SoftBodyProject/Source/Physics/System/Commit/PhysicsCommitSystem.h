#pragma once

#include "WorldStorage.h"
#include "RigidBodyStorage.h"

class PhysicsCommitSystem
{
public:
	// コンストラクタ
	PhysicsCommitSystem() = default;
	// 更新
	void FixedUpdate(RigidBodyStorage* _bodyStorage, WorldStorage* _worldStorage);
};
