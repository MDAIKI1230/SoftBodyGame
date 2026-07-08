#pragma once

#include <memory>

#include "WorldStorage.h"
#include "EventManager.h"

#include "CollisionManifoldBuffer.h"
#include "ColliderStorage.h"
#include "RigidBodyStorage.h"
#include "PhysicsTransformStorage.h"
#include "ConstraintStorage.h"

#include "AABBUpdateSystem.h"
#include "CollisionSystem.h"
#include "CollisionSolverSystem.h"
#include "PhysicsCommitSystem.h"
#include "SynchronizationSystem.h"
#include "RigidBodySystem.h"

class PhysicsWorld
{
public:
	// コンストラクタ
	PhysicsWorld();

	// 物理更新
	void FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager);

	// コライダーストレージ取得
	ColliderStorage* GetColliderStorage() { return colliderStorage.get(); }
	// RigidBodyストレージ取得
	RigidBodyStorage* GetRigidBodyStorage() { return rigidBodyStorage.get(); }
	// PhysicsTransformストレージ取得
	PhysicsTransformStorage* GetPhysicsTransformStorage() { return transformStorage.get(); }
	// Constraintストレージ取得
	ConstraintStorage* GetConstraintStorage() { return constraintStorage.get(); }
private:
	std::unique_ptr<CollisionManifoldBuffer> manifoldBuffer;
	std::unique_ptr<ColliderStorage> colliderStorage;
	std::unique_ptr<RigidBodyStorage> rigidBodyStorage;
	std::unique_ptr< PhysicsTransformStorage> transformStorage;
	std::unique_ptr< ConstraintStorage> constraintStorage;

	std::unique_ptr<AABBUpdateSystem> aabbUpdateSystem;
	std::unique_ptr<CollisionSystem> collisionSystem;
	std::unique_ptr<CollisionSolverSystem> collisionSolverSystem;
	std::unique_ptr<PhysicsCommitSystem> physicsCommitSystem;
	std::unique_ptr<SynchronizationSystem> synchronizationSystem;
	std::unique_ptr<RigidBodySystem> rigidBodySystem;
};
