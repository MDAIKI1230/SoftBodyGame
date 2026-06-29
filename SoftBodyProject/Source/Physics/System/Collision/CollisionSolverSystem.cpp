#include "CollisionSolverSystem.h"

void CollisionSolverSystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	VelocitySolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	PositionSolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	OrientationSolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	RotationSolver(_transformStorage, _bodyStorage, _manifoldBuffer);
}

void CollisionSolverSystem::PositionSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	int size{ (int)_manifoldBuffer->manifolds.size() };

	for (int i{ 0 }; i < size; i++)
	{
		Manifold& manifold{ _manifoldBuffer->manifolds[i] };
		Vector3 vec{ manifold.normal * manifold.points[0].penetration };

		_transformStorage->position[i] -= vec * 0.5f;
		_transformStorage->position[i] -= -vec * 0.5f;
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
