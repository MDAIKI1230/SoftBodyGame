#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	manifoldBuffer = std::make_unique<CollisionManifoldBuffer>();
	colliderStorage = std::make_unique<ColliderStorage>();
	rigidBodyStorage = std::make_unique<RigidBodyStorage>();
	transformStorage = std::make_unique<PhysicsTransformStorage>();

	aabbUpdateSystem = std::make_unique<AABBUpdateSystem>();
	collisionSystem = std::make_unique<CollisionSystem>();
	collisionSolverSystem = std::make_unique<CollisionSolverSystem>();
	physicsCommitSystem = std::make_unique<PhysicsCommitSystem>();
	synchronizationSystem = std::make_unique<SynchronizationSystem>();
	rigidBodySystem = std::make_unique<RigidBodySystem>();
}

void PhysicsWorld::FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	synchronizationSystem->Sync(_worldStorage, transformStorage.get());
	rigidBodySystem->FixedUpdate(rigidBodyStorage.get(), transformStorage.get());
	aabbUpdateSystem->FixedUpdate(colliderStorage.get(), _worldStorage);
	collisionSystem->FixedUpdate(_worldStorage, _eventManager, manifoldBuffer.get(), colliderStorage.get());
	// collisionSolverSystem->FixedUpdate(rigidBodyStorage.get(), manifoldBuffer.get());
	physicsCommitSystem->FixedUpdate(transformStorage.get(), _worldStorage);

	// 終了処理
	manifoldBuffer->manifolds.clear();
}
