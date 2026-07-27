#include "SolverBodyBuildSystem.h"

void SolverBodyBuildSystem::Build(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, SolverBodyBuffer* _solverBodyBuffer)
{
	// メモリの確保
	_solverBodyBuffer->Reserve(_transformStorage->CountID());

	// すべてのソルバ用Bodyの作成をする
	for (auto& id : _transformStorage->GetIDRange())
	{
		BodyID bodyID;
		// Bodyがあるかの確認
		if (_bodyStorage->TryGetRigidBodyID(id, bodyID))
		{
			// MAPを確認してあったらそれを使う
			if (_solverBodyBuffer->Has(id))
			{
				_solverBodyBuffer->GetIndex(id);
			}
			else
			{
				// SolverBodyがないので作る
				if (_bodyStorage->IsAlive(bodyID))
				{
					// Bodyある版の作成
					CreateSolverBody(_transformStorage, _bodyStorage, id, bodyID, _solverBodyBuffer);
				}
				else
				{
					// Bodyない版の作成
					CreateSolverBody(_transformStorage, id, _solverBodyBuffer);
				}
			}
		}
		else
		{
			// Bodyない版の作成
			CreateSolverBody(_transformStorage, id, _solverBodyBuffer);
		}
	}
}

void SolverBodyBuildSystem::CreateSolverBody(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, PhysicsTransformID _transformID, BodyID _bodyID, SolverBodyBuffer* _solverBodyBuffer)
{
	SolverBody body;

	// 情報をひとつづつ埋めていく
	body.bodyID = _bodyID;
	body.transformID = _transformID;
	uint32_t transformIndex{ _transformStorage->GetDenseIndex(_transformID) };
	body.pastPos = _bodyStorage->GetRigidBodyPastPosition(_bodyID);
	body.position = _transformStorage->GetPosition(transformIndex);
	body.velocity = _bodyStorage->GetRigidBodyVelocity(_bodyID);
	body.pastRot = _bodyStorage->GetRigidBodyPastRotation(_bodyID);
	body.rotation = _transformStorage->GetRotation(transformIndex);
	body.angularVelocity = _bodyStorage->GetRigidBodyAngularVelocity(_bodyID);
	body.inverseMass = _bodyStorage->GetRigidBodyInverseMass(_bodyID);
	body.localInverseInertiaTensor = _bodyStorage->GetRigidBodyLocalInverseInertiaTensor(_bodyID);

	// 追加
	_solverBodyBuffer->Add(_transformID, body);
}

void SolverBodyBuildSystem::CreateSolverBody(PhysicsTransformStorage* _transformStorage, PhysicsTransformID _transformID, SolverBodyBuffer* _solverBodyBuffer)
{
	SolverBody body;

	// 情報をひとつづつ埋めていく(Bodyが存在しない版)
	body.transformID = _transformID;
	uint32_t transformIndex{ _transformStorage->GetDenseIndex(_transformID) };
	body.pastPos = _transformStorage->GetPosition(transformIndex);
	body.position = _transformStorage->GetPosition(transformIndex);
	body.velocity = Vector3::ZERO;
	body.pastRot = _transformStorage->GetRotation(transformIndex);
	body.rotation = _transformStorage->GetRotation(transformIndex);
	body.angularVelocity = Vector3::ZERO;;
	body.inverseMass = 0;
	body.localInverseInertiaTensor = Matrix4x4::Zero();

	// 追加
	_solverBodyBuffer->Add(_transformID, body);
}
