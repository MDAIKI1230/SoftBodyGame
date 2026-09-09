#pragma once

#include <memory>

#include "WorldStorage.h"
#include "EventManager.h"

#include "CollisionManifoldBuffer.h"
#include "SolverBodyBuffer.h"
#include "ConstraintBuffer.h"
#include "PhysicsCommandBuffer.h"

#include "ColliderStorage.h"
#include "BodyStorage.h"
#include "PhysicsTransformStorage.h"
#include "ConstraintStorage.h"
#include "CharacterControllerStorage.h"

#include "ApplyPhysicsCommandBufferSystem.h"
#include "SynchronizationSystem.h"
#include "CharacterControllerSystem.h"
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
#include "ConstraintDebugRenderingSystem.h"
#include "ColliderDebugRenderingSystem.h"
#endif // DEBUG


class PhysicsWorld
{
	friend class PhysicsAPI;
	friend class PhysicsComponentAPI;

public:
	// 物理更新
	void FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager);

#ifdef _DEBUG
	// デバッグ描画
	void DebugRender();
#endif // _DEBUG

private:
	// コライダーストレージ取得
	ColliderStorage* GetColliderStorage() { return &colliderStorage; }
	// RigidBodyストレージ取得
	BodyStorage* GetRigidBodyStorage() { return &bodyStorage; }
	// PhysicsTransformストレージ取得
	PhysicsTransformStorage* GetPhysicsTransformStorage() { return &transformStorage; }
	// Constraintストレージ取得
	ConstraintStorage* GetConstraintStorage() { return &constraintStorage; }
	// CharacterControllerストレージ取得
	CharacterControllerStorage* GetCharacterControllerStorage() { return &characterControllerStorage; }

	void Solver();
private:
	// 解消回数
	static constexpr float SOLVER_TIMES{ 10 };
private:
	CollisionManifoldBuffer manifoldBuffer;
	SolverBodyBuffer solverBodyBuffer;
	ConstraintBuffer constraintBuffer;
	PhysicsCommandBuffer commandBuffer;

	ColliderStorage colliderStorage;
	BodyStorage bodyStorage;
	PhysicsTransformStorage transformStorage;
	ConstraintStorage constraintStorage;
	CharacterControllerStorage characterControllerStorage;

	ApplyPhysicsCommandBufferSystem applyPhysicsCommandBufferSystem;
	SynchronizationSystem synchronizationSystem;
	
	CharacterControllerSystem characterControllerSystem;
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
	ConstraintDebugRenderingSystem constraintDebugRenderingSystem;
	ColliderDebugRenderingSystem colliderDebugRenderingSystem;
#endif // _DEBUG
};
