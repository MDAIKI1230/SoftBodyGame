#include "CollisionSolverSystem.h"

void CollisionSolverSystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	StartUp(_transformStorage, _bodyStorage, _colliderStorage, _manifoldBuffer);
	VelocitySolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	// PositionSolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	OrientationSolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	RotationSolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	End(_transformStorage, _bodyStorage);
}

void CollisionSolverSystem::StartUp(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	// メモリの確保
	contactConstraints.reserve(_manifoldBuffer->manifolds.size() * 2);
	solverBodies.reserve(_manifoldBuffer->manifolds.size());
	bodyMap.reserve(_manifoldBuffer->manifolds.size());

	// すべての衝突情報から拘束条件とソルバ用Bodyの作成をする
	for (auto& manifold : _manifoldBuffer->manifolds)
	{
		for (int i{ 0 }; i < manifold.pointCount; i++)
		{
			ContactConstraint constraint;
			// SolverBodyのIndexを取得
			PhysicsTransformID transformID{ _colliderStorage->GetTransformID(manifold.colliderA) };
			constraint.solverBodyAIndex = GetSolverBodyIndex(_transformStorage, _bodyStorage, transformID);
			transformID = _colliderStorage->GetTransformID(manifold.colliderB);
			constraint.solverBodyBIndex = GetSolverBodyIndex(_transformStorage, _bodyStorage, transformID);

			constraint.normal = manifold.normal;
			constraint.penetration = manifold.points[i].penetration;

			contactConstraints.push_back(constraint);
		}
	}
}

void CollisionSolverSystem::PositionSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	//for (auto& contactConstraint : contactConstraints)
	//{
	//	// 質量から両者がBodyを持っているかの判定をする(どちらかがBodyを持っているなら合計は0じゃないはず)
	//	float totalInvMass{ solverBodies[contactConstraint.solverBodyAIndex].mass + solverBodies[contactConstraint.solverBodyBIndex].mass };
	//	if (totalInvMass == 0)
	//	{
	//		continue;
	//	}

	//	solverBodies[contactConstraint.solverBodyAIndex].position += contactConstraint.normal * contactConstraint.penetration * (solverBodies[contactConstraint.solverBodyAIndex].mass / totalInvMass);
	//	solverBodies[contactConstraint.solverBodyBIndex].position -= contactConstraint.normal * contactConstraint.penetration * (solverBodies[contactConstraint.solverBodyBIndex].mass / totalInvMass);
	//}
}

void CollisionSolverSystem::VelocitySolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	for (auto& contactConstraint : contactConstraints)
	{
		//float totalInvMass{ solverBodies[contactConstraint.solverBodyAIndex].inverseMass + solverBodies[contactConstraint.solverBodyBIndex].inverseMass };
		//if (totalInvMass == 0)
		//{
		//	continue;
		//}

		//// 相対速度
		//Vector3 relativevec{ solverBodies[contactConstraint.solverBodyAIndex].velocity - solverBodies[contactConstraint.solverBodyBIndex].velocity };
		//// 質量係数( mass01 * mass02 / mass01 + mass02)
		//float massCoefficient{
		//	(solverBodies[contactConstraint.solverBodyAIndex].mass * solverBodies[contactConstraint.solverBodyBIndex].mass)
		//	/ (solverBodies[contactConstraint.solverBodyAIndex].mass + solverBodies[contactConstraint.solverBodyBIndex].mass) };

		//solverBodies[]

		//if(_bodystorage->isalive(manifold.bodya))
		//{
		//	uint32_t index{ _bodystorage->getdenseindex(manifold.bodyid) };
		//	
		//	
		//	float j{ masscoefficient * vector3::dot(relativevec, manifold.normal) };
		//	_bodystorage->velocity[a.id] + (manifold.normal * j) / _bodystorage->mass[a.id];
		//	_bodystorage->velocity[b.id] - (manifold.normal * j) / _bodystorage->mass[b.id];
		//}
	}
}

void CollisionSolverSystem::OrientationSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{

}

void CollisionSolverSystem::RotationSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{

}

void CollisionSolverSystem::End(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage)
{
	// 結果を反映していく
	for (auto& result : solverBodies)
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
	contactConstraints.clear();
	solverBodies.clear();
	bodyMap.clear();
}

uint32_t CollisionSolverSystem::CreateSolverBody(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, PhysicsTransformID& _transformID, BodyID& _bodyID)
{
	SolverBody body;

	// 情報をひとつづつ埋めていく
	body.bodyID = _bodyID;
	body.transformID = _transformID;
	uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(_bodyID) };
	uint32_t transformIndex{ _transformStorage->GetDenseIndex(_transformID) };
	body.position = _transformStorage->position[transformIndex];
	body.velocity = _bodyStorage->velocity[bodyIndex];
	body.rotation = _transformStorage->rotation[transformIndex];
	body.angularVelocity = _bodyStorage->angularVelocity[bodyIndex];
	body.mass = _bodyStorage->mass[bodyIndex];
	body.inverseMass = _bodyStorage->inverseMass[bodyIndex];
	body.inverseInertiaTensor = _bodyStorage->worldInverseInertiaTensor[bodyIndex];

	// インデックス取ってから追加
	uint32_t result{ static_cast<uint32_t>(solverBodies.size()) };
	solverBodies.push_back(body);
	return result;
}

uint32_t CollisionSolverSystem::CreateSolverBody(PhysicsTransformStorage* _transformStorage, PhysicsTransformID& _transformID)
{
	SolverBody body;

	// 情報をひとつづつ埋めていく(Bodyが存在しない版)
	body.transformID = _transformID;
	uint32_t transformIndex{ _transformStorage->GetDenseIndex(_transformID) };
	body.position = _transformStorage->position[transformIndex];
	body.rotation = _transformStorage->rotation[transformIndex];
	body.mass = 0;
	body.inverseMass = 0;
	body.inverseInertiaTensor = Matrix4x4::Zero();

	// インデックス取ってから追加
	uint32_t result{ static_cast<uint32_t>(solverBodies.size()) };
	solverBodies.push_back(body);
	return result;
}

uint32_t   CollisionSolverSystem::GetSolverBodyIndex(
	PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, PhysicsTransformID& _transformID)
{
	uint32_t result;
	
	BodyID bodyID;
	// Bodyがあるかの確認
	if (_bodyStorage->TryGet(_transformID, bodyID))
	{
		// MAPを確認してあったらそれを使う
		if (bodyMap.contains(_transformID))
		{
			result = bodyMap[_transformID];
		}
		else
		{
			// SolverBodyがないので作る
			if (_bodyStorage->IsAlive(bodyID))
			{
				// Bodyある版の作成
				result = CreateSolverBody(_transformStorage, _bodyStorage, _transformID, bodyID);
				bodyMap[_transformID] = result;
			}
			else
			{
				// Bodyない版の作成
				result = CreateSolverBody(_transformStorage, _transformID);
				bodyMap[_transformID] = result;
			}
		}
	}
	else
	{
		// Bodyない版の作成
		result = CreateSolverBody(_transformStorage, _transformID);
		bodyMap[_transformID] = result;
	}

	return result;
}
