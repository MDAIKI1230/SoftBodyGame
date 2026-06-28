#include "CollisionSolverSystem.h"

void CollisionSolverSystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	PositionSolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	VelocitySolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	OrientationSolver(_transformStorage, _bodyStorage, _manifoldBuffer);
	RotationSolver(_transformStorage, _bodyStorage, _manifoldBuffer);
}

void CollisionSolverSystem::PositionSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	/*int size{ (int)_manifoldBuffer->manifolds.size() };

	for (int i{ 0 }; i < size; i++)
	{
		Manifold& manifold{ _manifoldBuffer->manifolds[i] };
		Vector3 vec{ manifold.normal * manifold.points[0].penetration };

		_bodyStorage->position[i] -= vec * 0.5f;
		_bodyStorage->position[i] -= -vec * 0.5f;
	}*/
}

void CollisionSolverSystem::VelocitySolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	//int size{ (int)_manifoldBuffer->manifolds.size() };

	//for (int i{ 0 }; i < size; i++)
	//{
	//	Manifold& manifold{ _manifoldBuffer->manifolds[i] };
	//	BodyID a{ manifold.bodyA };
	//	BodyID b{ manifold.bodyA };

	//	if(_bodyStorage->velocity.size() >= a.id && _bodyStorage->velocity.size() >= b.id)
	//	{
	//		Vector3 relativeVec{ _bodyStorage->velocity[b.id] - _bodyStorage->velocity[a.id] };
	//		float massCoefficient{ (_bodyStorage->mass[a.id] * _bodyStorage->mass[b.id]) / (_bodyStorage->mass[a.id] + _bodyStorage->mass[b.id]) };
	//		float j{ massCoefficient * Vector3::Dot(relativeVec, manifold.normal) };
	//		_bodyStorage->velocity[a.id] + (manifold.normal * j) / _bodyStorage->mass[a.id];
	//		_bodyStorage->velocity[b.id] - (manifold.normal * j) / _bodyStorage->mass[b.id];
	//	}
	//	else if(_bodyStorage->velocity.size() >= b.id)
	//	{
	//		// 質量は1と仮定
	//		float massCoefficient{ (_bodyStorage->mass[b.id]) / (_bodyStorage->mass[b.id] + 1) };
	//		float j{ massCoefficient * Vector3::Dot(_bodyStorage->velocity[b.id], manifold.normal) };
	//		_bodyStorage->velocity[b.id] + (manifold.normal * j) / _bodyStorage->mass[b.id];
	//	}
	//	else
	//	{
	//		// 質量は1と仮定
	//		float massCoefficient{ (_bodyStorage->mass[a.id]) / (_bodyStorage->mass[a.id] + 1) };
	//		float j{ massCoefficient * Vector3::Dot(_bodyStorage->velocity[a.id], manifold.normal) };
	//		_bodyStorage->velocity[a.id] + (manifold.normal * j) / _bodyStorage->mass[a.id];
	//	}
	//}
}

void CollisionSolverSystem::OrientationSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{

}

void CollisionSolverSystem::RotationSolver(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{

}
