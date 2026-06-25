#pragma once

#include "WorldStorage.h"
#include "CollisionManifoldBuffer.h"

class CollisionSolverSystem
{
public:
	void FixedUpdate(WorldStorage* _worldStorage, CollisionManifoldBuffer* _manifoldBuffer);
private:
	// 位置解決
	void PositionSolver();
	// 速度解決
	void VelocitySolver();
	// 姿勢解決
	void OrientationSolver();
	// 回転速度解決
	void RotationSolver();
};
