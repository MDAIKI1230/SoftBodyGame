#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	manifoldBuffer = std::make_unique<CollisionManifoldBuffer>();
	collisionSystem = std::make_unique<CollisionSystem>();
	collisionSolverSystem = std::make_unique<CollisionSolverSystem>();
	physicsCommitSystem = std::make_unique<PhysicsCommitSystem>();
	rigidBodySystem = std::make_unique<RigidBodySystem>();
}

void PhysicsWorld::FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	rigidBodySystem->FixedUpdate(_worldStorage, _eventManager);
	collisionSystem->FixedUpdate(_worldStorage, _eventManager);
	collisionSolverSystem->FixedUpdate(_worldStorage, _eventManager);
	physicsCommitSystem->FixedUpdate(_worldStorage, _eventManager);
}
