#include "PhysicsWorld.h"

void PhysicsWorld::FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	// 更新処理
	synchronizationSystem.Sync(_worldStorage, &transformStorage);
	rigidBodySystem.FixedUpdate(&transformStorage, &bodyStorage, &colliderStorage);
	aabbUpdateSystem.FixedUpdate(&transformStorage, &colliderStorage);

	// 当り判定
	collisionSystem.FixedUpdate(&transformStorage, &colliderStorage, &manifoldBuffer, _eventManager);

	// 衝突・拘束解消
	Solver();
	/*solverBodyBuildSystem->Build(transformStorage.get(), rigidBodyStorage.get(), solverBodyBuffer.get());
	collisionSolverSystem->FixedUpdate(colliderStorage.get(), manifoldBuffer.get(), solverBodyBuffer.get());
	solverBodyCommitSystem->Commit(transformStorage.get(), rigidBodyStorage.get(), solverBodyBuffer.get());
	solverBodyBuffer->Clear();*/
	// シミュレーション結果反映
	physicsCommitSystem.FixedUpdate(&transformStorage, _worldStorage);
}

#ifdef _DEBUG
void PhysicsWorld::DebugRender()
{
	constraintDebugRenderSystem.Render(&constraintStorage, &transformStorage);
}
#endif // _DEBUG

void PhysicsWorld::Solver()
{
	solverBodyBuildSystem.Build(&transformStorage, &bodyStorage, &solverBodyBuffer);
	// 解消準備
	collisionSolverSystem.StartUp(&colliderStorage, &manifoldBuffer, &solverBodyBuffer);

	// 速度解消を指定回数分回す
	for (int i{ 0 }; i < VELOCITY_SOLVER_TIMES; i++)
	{
		collisionSolverSystem.VelocitySolver(&manifoldBuffer, &solverBodyBuffer);

		// 拘束生成
		constraintBuildSystem.FixedUpdate(&constraintStorage, &solverBodyBuffer, &constraintBuffer);

		constraintSolverSystem.VelocitySolver(&solverBodyBuffer, &constraintBuffer);

		constraintBuffer.Clear();
	}

	// 修正された速度で位置を再計算
	collisionSolverSystem.ReCalcPosRot(&solverBodyBuffer);

	// 位置/姿勢解消を指定回数分回す
	/*for (int i{ 0 }; i < POS_ROT_SOLVER_TIMES; i++)
	{

		collisionSolverSystem->PositionSolver(manifoldBuffer.get(), solverBodyBuffer.get());

		 拘束生成
		constraintBuildSystem->FixedUpdate(constraintStorage.get(), solverBodyBuffer.get(), constraintBuffer.get());

		constraintSolverSystem->PositionSolver(solverBodyBuffer.get(), constraintBuffer.get());

		constraintBuffer->Clear();
	}*/

	// 終了
	collisionSolverSystem.End(&manifoldBuffer);

	solverBodyCommitSystem.Commit(&transformStorage, &bodyStorage, &solverBodyBuffer);
	solverBodyBuffer.Clear();
}
