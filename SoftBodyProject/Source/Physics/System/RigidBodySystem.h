#pragma once

#include "FixedUpdateSystem.h"

class RigidBodySystem:public FixedUpdateSystem
{
public:
	// コンストラクタ
	RigidBodySystem();
	// 更新
	void FixedUpdate(IWorld* _world)override;
private:
	// 重力適応
	void UpdateGravity();
	// 位置更新
	void UpdatePosition();
	// 回転更新
	void UpdateRotation();
};
