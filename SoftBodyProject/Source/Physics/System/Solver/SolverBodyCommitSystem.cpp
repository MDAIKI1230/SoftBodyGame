#include "SolverBodyCommitSystem.h"

void SolverBodyCommitSystem::Commit(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, SolverBodyBuffer* _solverBodyBuffer)
{
	// 結果を反映していく
	for (auto& result : _solverBodyBuffer->solverBodies)
	{
		// 質量が0ならBodyはないので書かない
		if (result.inverseMass > 0)
		{
			uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(result.bodyID) };
			uint32_t transformIndex{ _transformStorage->GetDenseIndex(result.transformID) };
			_transformStorage->position[transformIndex] = result.position;
			_bodyStorage->velocity[bodyIndex] = result.velocity;
			_transformStorage->rotation[transformIndex] = result.rotation;
			_bodyStorage->angularVelocity[bodyIndex] = result.angularVelocity;
		}
	}

	// リセット
	_solverBodyBuffer->Clear();
}
