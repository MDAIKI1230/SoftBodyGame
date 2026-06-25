#pragma once

#include "FixedUpdateSystem.h"

#include "RigidBodyComponentStorage.h"
#include "TransformComponentStorage.h"

class RigidBodySystem
{
public:
	// コンストラクタ
	RigidBodySystem() = default;
	// 更新
	void FixedUpdate(WorldStorage* _worldStorage);
private:
	// 重力適応
	void UpdateGravity(RigidBodyComponentStorage* _storage);
	// 位置更新
	void UpdatePosition(RigidBodyComponentStorage* _bodyStorage, TransformComponentStorage* _transformStorage);
	// 回転更新
	void UpdateRotation(RigidBodyComponentStorage* _bodyStorage, TransformComponentStorage* _transformStorage);
};
