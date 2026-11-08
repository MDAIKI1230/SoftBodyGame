#pragma once

#include "WorldStorage.h"
#include "PhysicsTransformStorage.h"
#include "TransformComponentStorage.h"

class PhysicsCommitSystem
{
public:
	// コンストラクタ
	PhysicsCommitSystem() = default;
	// 更新
	void FixedUpdate(PhysicsTransformStorage* _physicsTransformStorage, WorldStorage* _worldStorage);
private:
	// ECSの階層と今回の物理結果から、最新のワールド行列を求める。
	Matrix4x4 ResolveWorldMatrix(PhysicsTransformStorage* _physicsTransformStorage, TransformComponentStorage* _transformStorage, EntityID _entity);
};
