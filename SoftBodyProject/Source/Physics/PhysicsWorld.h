#pragma once

#include <memory>

#include "WorldStorage.h"
#include "EventManager.h"

#include "CollisionManifoldBuffer.h"
#include "SolverBodyBuffer.h"
#include "ConstraintBuffer.h"

#include "ColliderStorage.h"
#include "BodyStorage.h"
#include "PhysicsTransformStorage.h"
#include "ConstraintStorage.h"

#include "SynchronizationSystem.h"
#include "RigidBodySystem.h"
#include "AABBUpdateSystem.h"
#include "CollisionSystem.h"
#include "SolverBodyBuildSystem.h"
#include "CollisionSolverSystem.h"
#include "ConstraintBuildSystem.h"
#include "ConstraintSolverSystem.h"
#include "SolverBodyCommitSystem.h"
#include "PhysicsCommitSystem.h"

#ifdef _DEBUG
#include "ConstraintDebugRenderSystem.h"
#endif // DEBUG


class PhysicsWorld
{
public:
	// 物理更新
	void FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager);

#ifdef _DEBUG
	// デバッグ描画
	void DebugRender();
#endif // _DEBUG

	// コライダーストレージ取得
	ColliderStorage* GetColliderStorage() { return &colliderStorage; }
	// RigidBodyストレージ取得
	BodyStorage* GetRigidBodyStorage() { return &bodyStorage; }
	// PhysicsTransformストレージ取得
	PhysicsTransformStorage* GetPhysicsTransformStorage() { return &transformStorage; }
	// Constraintストレージ取得
	ConstraintStorage* GetConstraintStorage() { return &constraintStorage; }
private:
	void Solver();
private:
	// 速度解消回数
	static constexpr float VELOCITY_SOLVER_TIMES{ 10 };
	// 位置/姿勢解消回数
	static constexpr float POS_ROT_SOLVER_TIMES{ 4 };
private:
	CollisionManifoldBuffer manifoldBuffer;
	SolverBodyBuffer solverBodyBuffer;
	ConstraintBuffer constraintBuffer;


	ColliderStorage colliderStorage;
	BodyStorage bodyStorage;
	PhysicsTransformStorage transformStorage;
	ConstraintStorage constraintStorage;

	SynchronizationSystem synchronizationSystem;
	RigidBodySystem rigidBodySystem;
	AABBUpdateSystem aabbUpdateSystem;
	CollisionSystem collisionSystem;
	SolverBodyBuildSystem solverBodyBuildSystem;
	CollisionSolverSystem collisionSolverSystem;
	ConstraintBuildSystem constraintBuildSystem;
	ConstraintSolverSystem constraintSolverSystem;
	SolverBodyCommitSystem solverBodyCommitSystem;
	PhysicsCommitSystem physicsCommitSystem;

	// --- デバッグ用 ---
#ifdef _DEBUG
	ConstraintDebugRenderSystem constraintDebugRenderSystem;
#endif // _DEBUG
};
