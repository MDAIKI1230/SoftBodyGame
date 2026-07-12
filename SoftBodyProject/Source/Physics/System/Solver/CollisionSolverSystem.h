#pragma once

#include <vector>

#include "CollisionManifoldBuffer.h"
#include "SolverBodyBuffer.h"

#include "PhysicsTransformStorage.h"
#include "RigidBodyStorage.h"
#include "ColliderStorage.h"

#include "ContactConstraint.h"
#include "Constraint.h"
#include "SolverBody.h"

class CollisionSolverSystem
{
public:
	// コンストラクタ
	CollisionSolverSystem();

	// 準備
	void StartUp(ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer, SolverBodyBuffer* _solverBodyBuffer);
	// 速度/角速度解決
	void VelocitySolver(CollisionManifoldBuffer* _manifoldBuffer, SolverBodyBuffer* _solverBodyBuffer);
	// 摩擦計算
	void FrictionSolver(SolverBody& _bodyA, Vector3& _rA, SolverBody& _bodyB, Vector3& _rB, ContactConstraint& _constraint);
	// 位置姿勢の再計算
	void ReCalcPosRot(SolverBodyBuffer* _solverBodyBuffer);
	// 位置解決
	void PositionSolver(CollisionManifoldBuffer* _manifoldBuffer, SolverBodyBuffer* _solverBodyBuffer);

	// 終わり
	void End(CollisionManifoldBuffer* _manifoldBuffer);
private:
	// バネ定数
	const float K;
	// バネ定数×Δt
	const float K_DELTA_TIME;
	// 減衰定数
	const float C;
	// erp項
	const float ERP;
	// γ
	const float GAMMA;
	// 位置解消パーセント
	static constexpr float POSITION_SOLVE_PERCENT{ 0.2f };
	// 位置解消時のちょっとの余裕
	static constexpr float POSITION_SOLVE_SLOP{ 0.01f };
private:
	std::vector<ContactConstraint> contactConstraints;
	std::vector<Constraint> constraints;
};
