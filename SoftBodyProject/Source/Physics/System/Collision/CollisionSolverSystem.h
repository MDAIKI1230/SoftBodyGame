#pragma once

#include "CollisionManifoldBuffer.h"

#include "PhysicsTransformStorage.h"
#include "RigidBodyStorage.h"

class CollisionSolverSystem
{
public:
	// 更新
	void FixedUpdate(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
private:
	// 位置解決
	void PositionSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
	// 速度解決
	void VelocitySolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
	// 姿勢解決
	void OrientationSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
	// 回転速度解決
	void RotationSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
};
