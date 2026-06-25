#pragma once

#include "FixedUpdateSystem.h"

class CollisionSolverSystem :public FixedUpdateSystem
{
public:
	void FixedUpdate(WorldStorage* world) override;
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
