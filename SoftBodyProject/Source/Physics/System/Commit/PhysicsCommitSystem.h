#pragma once

#include "FixedUpdateSystem.h"

class PhysicsCommitSystem
{
public:
	// コンストラクタ
	PhysicsCommitSystem() = default;
	// 更新
	void FixedUpdate(WorldStorage* _worldStorage);
};
