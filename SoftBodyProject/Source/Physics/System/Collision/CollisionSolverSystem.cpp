#include "CollisionSolverSystem.h"

void CollisionSolverSystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	StartUp(_transformStorage, _bodyStorage, _manifoldBuffer);
	VelocitySolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	PositionSolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	OrientationSolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	RotationSolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	End(_transformStorage, _bodyStorage);
}

void CollisionSolverSystem::StartUp(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	contactConstraints.reserve(_manifoldBuffer->manifolds.size() * 2);
	solverBodies.reserve(_manifoldBuffer->manifolds.size());

	for (auto& manifold : _manifoldBuffer->manifolds)
	{
		for (int i{ 0 }; i < manifold.pointCount; i++)
		{
			ContactConstraint constraint;
			PhysicsTransformID transformID;
			if (_bodyStorage->TryGetTransformID(manifold.bodyA, transformID))
			{
				constraint.solverBodyAIndex = GetSolverBodyIndex(_transformStorage, _bodyStorage, transformID, manifold.bodyA);
			}
			
			if (_bodyStorage->TryGetTransformID(manifold.bodyB, transformID))
			{
				constraint.solverBodyBIndex = GetSolverBodyIndex(_transformStorage, _bodyStorage, transformID, manifold.bodyB);
			}

			constraint.normal = manifold.normal;
			constraint.penetration = manifold.points[i].penetration;

			contactConstraints.push_back(constraint);
		}
	}
}

void CollisionSolverSystem::PositionSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	for (auto& contactConstraint : contactConstraints)
	{
		float totalInvMass{ solverBodies[contactConstraint.solverBodyAIndex].inverseMass + solverBodies[contactConstraint.solverBodyBIndex].inverseMass };
		if (solverBodies[contactConstraint.solverBodyAIndex].inverseMass == 0 && solverBodies[contactConstraint.solverBodyBIndex].inverseMass == 0)
		{
			continue;
		}

		solverBodies[contactConstraint.solverBodyAIndex].position += contactConstraint.normal * contactConstraint.penetration * (solverBodies[contactConstraint.solverBodyAIndex].inverseMass / totalInvMass);
		solverBodies[contactConstraint.solverBodyBIndex].position -= contactConstraint.normal * contactConstraint.penetration * (solverBodies[contactConstraint.solverBodyBIndex].inverseMass / totalInvMass);
	}
}

void CollisionSolverSystem::VelocitySolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	int size{ (int)_manifoldBuffer->manifolds.size() };

	for (int i{ 0 }; i < size; i++)
	{
		Manifold& manifold{ _manifoldBuffer->manifolds[i] };

		//if(_bodyStorage->IsAlive(manifold.bodyA))
		//{
		//	uint32_t index{ _bodyStorage->GetDenseIndex(manifold.bodyID) };
		//	Vector3 relativeVec{ _bodyStorage->velocity[index] - _bodyStorage->velocity[index] };
		//	float massCoefficient{ (_bodyStorage->mass[index * _bodyStorage->mass[index]) / (_bodyStorage->mass[index] + _bodyStorage->mass[b.id]) };
		//	float j{ massCoefficient * Vector3::Dot(relativeVec, manifold.normal) };
		//	_bodyStorage->velocity[a.id] + (manifold.normal * j) / _bodyStorage->mass[a.id];
		//	_bodyStorage->velocity[b.id] - (manifold.normal * j) / _bodyStorage->mass[b.id];
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
		if (_bodyStorage->IsAlive(result.bodyID))
		{
			if (result.writeBack)
			{
				uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(result.bodyID) };
				uint32_t transformIndex{ _transformStorage->GetDenseIndex(result.transformID) };
				_transformStorage->position[transformIndex] = result.position;
				_bodyStorage->velocity[bodyIndex] = result.velocity;
				_transformStorage->rotation[transformIndex] = result.rotation;
				_bodyStorage->angularVelocity[bodyIndex] = result.angularVelocity;
			}
		}
	}
	// リセット
	contactConstraints.clear();
	solverBodies.clear();
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
	body.inverseMass = _bodyStorage->inverseMass[bodyIndex];
	body.inverseInertiaTensor = _bodyStorage->inverseInertiaTensor[bodyIndex];

	body.writeBack = true;

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
	body.inverseMass = 0;
	body.inverseInertiaTensor = Matrix4x4::Zero();

	body.writeBack = false;

	// インデックス取ってから追加
	uint32_t result{ static_cast<uint32_t>(solverBodies.size()) };
	solverBodies.push_back(body);
	return result;
}

uint32_t   CollisionSolverSystem::GetSolverBodyIndex(
	PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, BodyID& _bodyID)
{
	uint32_t result;
	
	PhysicsTransformID transformID;
	if (_bodyStorage->TryGetTransformID(_bodyID, transformID))
	{
		// MAPを確認してあったらそれを使う
		if (bodyMap.contains(transformID))
		{
			result = bodyMap[transformID];
		}
		else
		{
			// SolverBodyがないので作る
			if (_bodyStorage->IsAlive(_bodyID))
			{
				result = CreateSolverBody(_transformStorage, _bodyStorage, transformID, _bodyID);
				bodyMap[transformID] = result;
			}
			else
			{
				result = CreateSolverBody(_transformStorage, transformID);
				bodyMap[transformID] = result;
			}
		}
	}



	return result;
}
