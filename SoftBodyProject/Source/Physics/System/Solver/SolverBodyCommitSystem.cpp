#include "SolverBodyCommitSystem.h"

void SolverBodyCommitSystem::Commit(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, SolverBodyBuffer* _solverBodyBuffer)
{
	// 結果を反映していく
	for (auto& result : _solverBodyBuffer->GetAll())
	{
		// 質量が0ならBodyはないので書かない
		if (result.inverseMass > 0)
		{
			PhysicsTransformID transformID{ result.transformID };

			_transformStorage->EditPosition(transformID) = result.position;
			_bodyStorage->EditRigidBodyVelocity(result.bodyID) = result.velocity;
			_transformStorage->EditRotation(transformID) = result.rotation;
			_bodyStorage->EditRigidBodyAngularVelocity(result.bodyID) = result.angularVelocity;
		}
	}

	// リセット
	_solverBodyBuffer->Clear();
}
