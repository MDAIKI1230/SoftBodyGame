#include "SolverBodyBuildSystem.h"

void SolverBodyBuildSystem::Build(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, SolverBodyBuffer* _solverBodyBuffer)
{
	// メモリの確保
	_solverBodyBuffer->solverBodies.reserve(_transformStorage->id.size());
	_solverBodyBuffer->bodyMap.reserve(_transformStorage->id.size());

	// すべての衝突情報から拘束条件とソルバ用Bodyの作成をする
	for (auto& transformID : _transformStorage->id)
	{
		uint32_t result;

		BodyID bodyID;
		// Bodyがあるかの確認
		if (_bodyStorage->TryGetRigidBodyID(transformID, bodyID))
		{
			// MAPを確認してあったらそれを使う
			if (_solverBodyBuffer->bodyMap.contains(transformID))
			{
				result = _solverBodyBuffer->bodyMap[transformID];
			}
			else
			{
				// SolverBodyがないので作る
				if (_bodyStorage->IsAlive(bodyID))
				{
					// Bodyある版の作成
					result = CreateSolverBody(_transformStorage, _bodyStorage, transformID, bodyID, _solverBodyBuffer);
					_solverBodyBuffer->bodyMap[transformID] = result;
				}
				else
				{
					// Bodyない版の作成
					result = CreateSolverBody(_transformStorage, transformID, _solverBodyBuffer);
					_solverBodyBuffer->bodyMap[transformID] = result;
				}
			}
		}
		else
		{
			// Bodyない版の作成
			result = CreateSolverBody(_transformStorage, transformID, _solverBodyBuffer);
			_solverBodyBuffer->bodyMap[transformID] = result;
		}
	}
}

uint32_t SolverBodyBuildSystem::CreateSolverBody(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, PhysicsTransformID _transformID, BodyID _bodyID, SolverBodyBuffer* _solverBodyBuffer)
{
	SolverBody body;

	// 情報をひとつづつ埋めていく
	body.bodyID = _bodyID;
	body.transformID = _transformID;
	uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(_bodyID) };
	uint32_t transformIndex{ _transformStorage->GetDenseIndex(_transformID) };
	body.pastPos = _bodyStorage->rigidBodyStorage->pastPos[bodyIndex];
	body.position = _transformStorage->position[transformIndex];
	body.velocity = _bodyStorage->rigidBodyStorage->velocity[bodyIndex];
	body.pastRot = _bodyStorage->rigidBodyStorage->pastRot[bodyIndex];
	body.rotation = _transformStorage->rotation[transformIndex];
	body.angularVelocity = _bodyStorage->rigidBodyStorage->angularVelocity[bodyIndex];
	body.inverseMass = _bodyStorage->rigidBodyStorage->inverseMass[bodyIndex];
	body.localInverseInertiaTensor = _bodyStorage->rigidBodyStorage->localInverseInertiaTensor[bodyIndex];

	// インデックス取ってから追加
	uint32_t result{ static_cast<uint32_t>(_solverBodyBuffer->solverBodies.size()) };
	_solverBodyBuffer->solverBodies.push_back(body);
	return result;
}

uint32_t SolverBodyBuildSystem::CreateSolverBody(PhysicsTransformStorage* _transformStorage, PhysicsTransformID _transformID, SolverBodyBuffer* _solverBodyBuffer)
{
	SolverBody body;

	// 情報をひとつづつ埋めていく(Bodyが存在しない版)
	body.transformID = _transformID;
	uint32_t transformIndex{ _transformStorage->GetDenseIndex(_transformID) };
	body.pastPos = _transformStorage->position[transformIndex];
	body.position = _transformStorage->position[transformIndex];
	body.velocity = Vector3::ZERO;
	body.pastRot = _transformStorage->rotation[transformIndex];
	body.rotation = _transformStorage->rotation[transformIndex];
	body.angularVelocity = Vector3::ZERO;;
	body.inverseMass = 0;
	body.localInverseInertiaTensor = Matrix4x4::Zero();

	// インデックス取ってから追加
	uint32_t result{ static_cast<uint32_t>(_solverBodyBuffer->solverBodies.size()) };
	_solverBodyBuffer->solverBodies.push_back(body);
	return result;
}
