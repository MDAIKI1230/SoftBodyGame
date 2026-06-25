#pragma once

#include "FixedUpdateSystem.h"

class PhysicsCommitSystem :public FixedUpdateSystem
{
public:
	// コンストラクタ
	PhysicsCommitSystem();
	// 更新
	void FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager) override;
};
