#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
{
	manifoldBuffer = std::make_unique<CollisionManifoldBuffer>();
	solverBodyBuffer = std::make_unique<SolverBodyBuffer>();
	constraintBuffer = std::make_unique<ConstraintBuffer>();

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
	constraintBuildSystem = std::make_unique<ConstraintBuildSystem>();
	constraintSolverSystem = std::make_unique<ConstraintSolverSystem>();
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
	Solver();
	/*solverBodyBuildSystem->Build(transformStorage.get(), rigidBodyStorage.get(), solverBodyBuffer.get());
	collisionSolverSystem->FixedUpdate(colliderStorage.get(), manifoldBuffer.get(), solverBodyBuffer.get());
	solverBodyCommitSystem->Commit(transformStorage.get(), rigidBodyStorage.get(), solverBodyBuffer.get());
	solverBodyBuffer->Clear();*/
	// シミュレーション結果反映
	physicsCommitSystem->FixedUpdate(transformStorage.get(), _worldStorage);
}

void PhysicsWorld::Solver()
{
	solverBodyBuildSystem->Build(transformStorage.get(), rigidBodyStorage.get(), solverBodyBuffer.get());
	// 解消準備
	collisionSolverSystem->StartUp(colliderStorage.get(), manifoldBuffer.get(), solverBodyBuffer.get());
	// 拘束生成
	constraintBuildSystem->FixedUpdate(constraintStorage.get(), solverBodyBuffer.get(), constraintBuffer.get());
	// 速度解消を指定回数分回す
	for (int i{ 0 }; i < 10; i++)
	{
		collisionSolverSystem->VelocitySolver(manifoldBuffer.get(), solverBodyBuffer.get());
		constraintSolverSystem->ConstraintSolver(solverBodyBuffer.get(), constraintBuffer.get());
	}
	// 修正された速度で位置を再計算
	collisionSolverSystem->ReCalcPosRot(solverBodyBuffer.get());

	constraintBuffer->Clear();

	constraintBuildSystem->FixedUpdate(constraintStorage.get(), solverBodyBuffer.get(), constraintBuffer.get());
	// 位置/姿勢解消を指定回数分回す
	for (int i{ 0 }; i < 4; i++)
	{

		collisionSolverSystem->PositionSolver(manifoldBuffer.get(), solverBodyBuffer.get());

		constraintSolverSystem->PositionSolver(solverBodyBuffer.get(), constraintBuffer.get());


	}
	constraintBuffer->Clear();

	// 終了
	collisionSolverSystem->End(manifoldBuffer.get());

	solverBodyCommitSystem->Commit(transformStorage.get(), rigidBodyStorage.get(), solverBodyBuffer.get());
	solverBodyBuffer->Clear();
}
