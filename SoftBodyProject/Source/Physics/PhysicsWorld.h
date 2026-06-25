#pragma once

#include <memory>

#include "WorldStorage.h"
#include "EventManager.h"

#include "CollisionManifoldBuffer.h"

#include "CollisionSystem.h"
#include "CollisionSolverSystem.h"
#include "PhysicsCommitSystem.h"
#include "RigidBodySystem.h"

class PhysicsWorld
{
public:
	// コンストラクタ
	PhysicsWorld();

	// 物理更新
	void FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager);

private:
	std::unique_ptr<CollisionManifoldBuffer> manifoldBuffer;
	std::unique_ptr<CollisionSystem> collisionSystem;
	std::unique_ptr<CollisionSolverSystem> collisionSolverSystem;
	std::unique_ptr<PhysicsCommitSystem> physicsCommitSystem;
	std::unique_ptr<RigidBodySystem> rigidBodySystem;
};
