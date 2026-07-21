#pragma once

#include <memory>

#include "WorldStorage.h"
#include "EventManager.h"

#include "CollisionManifoldBuffer.h"
#include "SolverBodyBuffer.h"
#include "ConstraintBuffer.h"

#include "ColliderStorage.h"
#include "RigidBodyStorage.h"
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
	// コンストラクタ
	PhysicsWorld();

	// 物理更新
	void FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager);

#ifdef _DEBUG
	// デバッグ描画
	void DebugRender();
#endif // _DEBUG

	// コライダーストレージ取得
	ColliderStorage* GetColliderStorage() { return colliderStorage.get(); }
	// RigidBodyストレージ取得
	RigidBodyStorage* GetRigidBodyStorage() { return rigidBodyStorage.get(); }
	// PhysicsTransformストレージ取得
	PhysicsTransformStorage* GetPhysicsTransformStorage() { return transformStorage.get(); }
	// Constraintストレージ取得
	ConstraintStorage* GetConstraintStorage() { return constraintStorage.get(); }
private:
	void Solver();
private:
	// 速度解消回数
	static constexpr float VELOCITY_SOLVER_TIMES{ 10 };
	// 位置/姿勢解消回数
	static constexpr float POS_ROT_SOLVER_TIMES{ 4 };
private:
	std::unique_ptr<CollisionManifoldBuffer> manifoldBuffer;
	std::unique_ptr<SolverBodyBuffer> solverBodyBuffer;
	std::unique_ptr<ConstraintBuffer> constraintBuffer;


	std::unique_ptr<ColliderStorage> colliderStorage;
	std::unique_ptr<RigidBodyStorage> rigidBodyStorage;
	std::unique_ptr< PhysicsTransformStorage> transformStorage;
	std::unique_ptr< ConstraintStorage> constraintStorage;

	std::unique_ptr<SynchronizationSystem> synchronizationSystem;
	std::unique_ptr<RigidBodySystem> rigidBodySystem;
	std::unique_ptr<AABBUpdateSystem> aabbUpdateSystem;
	std::unique_ptr<CollisionSystem> collisionSystem;
	std::unique_ptr<SolverBodyBuildSystem> solverBodyBuildSystem;
	std::unique_ptr<CollisionSolverSystem> collisionSolverSystem;
	std::unique_ptr<ConstraintBuildSystem> constraintBuildSystem;
	std::unique_ptr<ConstraintSolverSystem> constraintSolverSystem;
	std::unique_ptr<SolverBodyCommitSystem> solverBodyCommitSystem;
	std::unique_ptr<PhysicsCommitSystem> physicsCommitSystem;

	// --- デバッグ用 ---
#ifdef _DEBUG
	std::unique_ptr<ConstraintDebugRenderSystem> constraintDebugRenderSystem;
#endif // _DEBUG
};
