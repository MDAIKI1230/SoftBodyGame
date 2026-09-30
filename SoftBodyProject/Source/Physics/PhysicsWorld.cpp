#include "PhysicsWorld.h"

void PhysicsWorld::Initialize()
{
	// 速度を変更しない衝突の解消を行って、速度が暴走するのを防ぐ。
	solverBodyBuildSystem.Build(&transformStorage, &bodyStorage, &solverBodyBuffer);
	
	collisionSolverSystem.StartUp(&colliderStorage, &manifoldBuffer, &solverBodyBuffer);

	for (int i{ 0 }; i < SOLVER_TIMES; i++)
	{
		constraintBuildSystem.Build(&constraintStorage, &solverBodyBuffer, &constraintBuffer);
		constraintSolverSystem.PBDPositionSolver(&solverBodyBuffer, &constraintBuffer);
		constraintBuffer.Clear();
	}

	solverBodyCommitSystem.Commit(&transformStorage, &bodyStorage, &solverBodyBuffer);
	solverBodyBuffer.Clear();
}

void PhysicsWorld::FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	// コマンドバッファ処理
	applyPhysicsCommandBufferSystem.Apply(&commandBuffer, &transformStorage, &bodyStorage);
	// 更新処理
	synchronizationSystem.Sync(_worldStorage, &transformStorage);
	aabbUpdateSystem.FixedUpdate(&transformStorage, &colliderStorage);
	characterControllerSystem.FixedUpdate(&characterControllerStorage, &transformStorage, &bodyStorage, &colliderStorage);
	rigidBodySystem.FixedUpdate(&transformStorage, &bodyStorage, &colliderStorage);
	aabbUpdateSystem.FixedUpdate(&transformStorage, &colliderStorage);

	// 当り判定
	collisionSystem.FixedUpdate(&transformStorage, &colliderStorage, &manifoldBuffer, _eventManager);

	// 衝突・拘束解消
	Solver();

	// シミュレーション結果反映
	physicsCommitSystem.FixedUpdate(&transformStorage, _worldStorage);
}

#ifdef _DEBUG
void PhysicsWorld::DebugRender()
{
	constraintDebugRenderingSystem.Render(&transformStorage, &constraintStorage);
	colliderDebugRenderingSystem.Render(&transformStorage, &colliderStorage);
}
#endif // _DEBUG

void PhysicsWorld::Solver()
{
	solverBodyBuildSystem.Build(&transformStorage, &bodyStorage, &solverBodyBuffer);
	// 解消準備
	collisionSolverSystem.StartUp(&colliderStorage, &manifoldBuffer, &solverBodyBuffer);

	// 拘束生成
	//constraintBuildSystem.FixedUpdate(&constraintStorage, &solverBodyBuffer, &constraintBuffer);

	//// 速度解消を指定回数分回す
	//for (int i{ 0 }; i < SOLVER_TIMES; i++)
	//{
	//	collisionSolverSystem.Solve(&manifoldBuffer, &solverBodyBuffer);

	//	constraintSolverSystem.Solve(&solverBodyBuffer, &constraintBuffer);
	//}

	//// 修正された速度で位置を再計算
	//collisionSolverSystem.ReCalcPosRot(&solverBodyBuffer);

	//for (int i{ 0 }; i < SOLVER_TIMES; i++)
	//{
	//	constraintBuildSystem.FixedUpdate(&constraintStorage, &solverBodyBuffer, &constraintBuffer);
	//	collisionSolverSystem.PositionSolver(&manifoldBuffer, &solverBodyBuffer);
	//	constraintSolverSystem.PositionSolver(&solverBodyBuffer, &constraintBuffer);
	//	constraintBuffer.Clear();
	//}
	// PBD版

	for (int i{ 0 }; i < SOLVER_TIMES; i++)
	{
		collisionSolverSystem.Solve(&manifoldBuffer, &solverBodyBuffer);
	}

	// 修正された速度で位置を再計算
	collisionSolverSystem.ReCalcPosRot(&solverBodyBuffer);

	constraintBuildSystem.Build(&constraintStorage, &solverBodyBuffer, &constraintBuffer);

	for (int i{ 0 }; i < SOLVER_TIMES; i++)
	{
		collisionSolverSystem.PositionSolver(&manifoldBuffer, &solverBodyBuffer);
		constraintBuildSystem.RefreshRows(&constraintStorage, &solverBodyBuffer, &constraintBuffer);
		constraintSolverSystem.PBDPositionSolver(&solverBodyBuffer, &constraintBuffer);
	}

	constraintSolverSystem.ReCalcVelocity(&solverBodyBuffer);

	// 終了
	collisionSolverSystem.End(&manifoldBuffer);
	constraintBuffer.Clear();

	solverBodyCommitSystem.Commit(&transformStorage, &bodyStorage, &solverBodyBuffer);
	solverBodyBuffer.Clear();
}