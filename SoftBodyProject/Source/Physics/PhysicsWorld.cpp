#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	manifoldBuffer = std::make_unique<CollisionManifoldBuffer>();
	solverBodyBuffer = std::make_unique<SolverBodyBuffer>();

	colliderStorage = std::make_unique<ColliderStorage>();
	rigidBodyStorage = std::make_unique<RigidBodyStorage>();
	transformStorage = std::make_unique<PhysicsTransformStorage>();
	constraintStorage = std::make_unique<ConstraintStorage>();

	synchronizationSystem = std::make_unique<SynchronizationSystem>();
	rigidBodySystem = std::make_unique<RigidBodySystem>();
	aabbUpdateSystem = std::make_unique<AABBUpdateSystem>();
	collisionSystem = std::make_unique<CollisionSystem>();
	solverBodyBuildSystem = std::make_unique<SolverBodyBuildSystem>();
	collisionSolverSystem = std::make_unique<CollisionSolverSystem>();
	solverBodyCommitSystem = std::make_unique<SolverBodyCommitSystem>();
	physicsCommitSystem = std::make_unique<PhysicsCommitSystem>();
}

void PhysicsWorld::FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	// 更新処理
	synchronizationSystem->Sync(_worldStorage, transformStorage.get());
	rigidBodySystem->FixedUpdate(transformStorage.get(), rigidBodyStorage.get(), colliderStorage.get());
	aabbUpdateSystem->FixedUpdate(transformStorage.get(), colliderStorage.get());

	// 当り判定
	collisionSystem->FixedUpdate(transformStorage.get(), colliderStorage.get(), manifoldBuffer.get(), _eventManager);

	// 衝突・拘束解消
	solverBodyBuildSystem->Build(transformStorage.get(), rigidBodyStorage.get(), solverBodyBuffer.get());
	collisionSolverSystem->FixedUpdate(colliderStorage.get(), manifoldBuffer.get(), solverBodyBuffer.get());
	solverBodyCommitSystem->Commit(transformStorage.get(), rigidBodyStorage.get(), solverBodyBuffer.get());

	// シミュレーション結果反映
	physicsCommitSystem->FixedUpdate(transformStorage.get(), _worldStorage);
}
