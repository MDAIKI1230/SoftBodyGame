#pragma once

#include "RigidBodyStorage.h"
#include "PhysicsTransformStorage.h"

class RigidBodySystem
{
public:
	// コンストラクタ
	RigidBodySystem() = default;
	// 更新
	void FixedUpdate(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage);
private:
	// 重力適応
	void UpdateGravity(RigidBodyStorage* _bodyStorage);
	// 位置更新
	void UpdatePosition(RigidBodyStorage* _bodyStorage, PhysicsTransformStorage* _transformStorage);
	// 回転更新
	void UpdateRotation(RigidBodyStorage* _bodyStorage, PhysicsTransformStorage* _transformStorage);
};
