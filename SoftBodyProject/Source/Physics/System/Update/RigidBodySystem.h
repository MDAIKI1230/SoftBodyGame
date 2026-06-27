#pragma once

#include "FixedUpdateSystem.h"

#include "RigidBodyStorage.h"

class RigidBodySystem
{
public:
	// コンストラクタ
	RigidBodySystem() = default;
	// 更新
	void FixedUpdate(RigidBodyStorage* _bodyStorage);
private:
	// 重力適応
	void UpdateGravity(RigidBodyStorage* _bodyStorage);
	// 位置更新
	void UpdatePosition(RigidBodyStorage* _bodyStorage);
	// 回転更新
	void UpdateRotation(RigidBodyStorage* _bodyStorage);
};
