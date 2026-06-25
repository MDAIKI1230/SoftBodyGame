#pragma once

#include "WorldStorage.h"
#include "CollisionManifoldBuffer.h"

#include "RigidBodyComponentStorage.h"

class CollisionSolverSystem
{
public:
	void FixedUpdate(WorldStorage* _worldStorage, CollisionManifoldBuffer* _manifoldBuffer);
private:
	// 位置解決
	void PositionSolver(RigidBodyComponentStorage* _bodyStorage);
	// 速度解決
	void VelocitySolver(RigidBodyComponentStorage* _bodyStorage);
	// 姿勢解決
	void OrientationSolver(RigidBodyComponentStorage* _bodyStorage);
	// 回転速度解決
	void RotationSolver(RigidBodyComponentStorage* _bodyStorage);
};
