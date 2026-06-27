#pragma once

#include "WorldStorage.h"
#include "CollisionManifoldBuffer.h"

#include "RigidBodyStorage.h"

class CollisionSolverSystem
{
public:
	// 更新
	void FixedUpdate(RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
private:
	// 位置解決
	void PositionSolver(RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
	// 速度解決
	void VelocitySolver(RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
	// 姿勢解決
	void OrientationSolver(RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
	// 回転速度解決
	void RotationSolver(RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer);
};
