#include "SolverBodyCommitSystem.h"

void SolverBodyCommitSystem::Commit(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, SolverBodyBuffer* _solverBodyBuffer)
{
	// 結果を反映していく
	for (auto& result : _solverBodyBuffer->solverBodies)
	{
		// 質量が0ならBodyはないので書かない
		if (result.inverseMass > 0)
		{
			uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(result.bodyID) };
			uint32_t transformIndex{ _transformStorage->GetDenseIndex(result.transformID) };
			_transformStorage->EditPosition(transformIndex) = result.position;
			_bodyStorage->rigidBodyStorage->velocity[bodyIndex] = result.velocity;
			_transformStorage->rotation[transformIndex] = result.rotation;
			_bodyStorage->rigidBodyStorage->angularVelocity[bodyIndex] = result.angularVelocity;
		}
	}

	// リセット
	_solverBodyBuffer->Clear();
}
