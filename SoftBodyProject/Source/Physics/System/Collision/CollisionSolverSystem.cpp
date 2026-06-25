#include "CollisionSolverSystem.h"

void CollisionSolverSystem::FixedUpdate(WorldStorage* _worldStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(_worldStorage->GetStorage<RigidBodyComponent>()) };

	PositionSolver(bodyStorage, _manifoldBuffer);
	VelocitySolver(bodyStorage, _manifoldBuffer);
	OrientationSolver(bodyStorage, _manifoldBuffer);
	RotationSolver(bodyStorage, _manifoldBuffer);
}

void CollisionSolverSystem::PositionSolver(RigidBodyComponentStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	int size{ _manifoldBuffer->manifolds.size() };

	for (int i{ size }; i < size; i++)
	{
		Manifold& manifold{ _manifoldBuffer->manifolds[i] };
		Vector3 vec{ manifold.normal * manifold.points[0].penetration };

		_bodyStorage->expectedPos[manifold.handleA] -= vec;
		_bodyStorage->expectedPos[manifold.handleB] -= -vec;
	}
}

void CollisionSolverSystem::VelocitySolver(RigidBodyComponentStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	int size{ _manifoldBuffer->manifolds.size() };

	for (int i{ size }; i < size; i++)
	{
		Manifold& manifold{ _manifoldBuffer->manifolds[i] };
		int a{ manifold.handleA };
		int b{ manifold.handleB };

		// どちらかRigidBodyがないならaが-1になる。
		if(a != -1)
		{
			Vector3 relativeVec{ _bodyStorage->velocity[b] - _bodyStorage->velocity[a] };
			float massCoefficient{ (_bodyStorage->mass[a] * _bodyStorage->mass[b]) / (_bodyStorage->mass[a] + _bodyStorage->mass[b]) };
			float j{ massCoefficient * Vector3::Dot(relativeVec, manifold.normal) };
			_bodyStorage->velocity[a] + (manifold.normal * j) / _bodyStorage->mass[a];
			_bodyStorage->velocity[b] - (manifold.normal * j) / _bodyStorage->mass[b];
		}
		else
		{
			// 質量は1と仮定
			float massCoefficient{ (_bodyStorage->mass[b]) / (_bodyStorage->mass[b] + 1) };
			float j{ massCoefficient * Vector3::Dot(_bodyStorage->velocity[b], manifold.normal) };
			_bodyStorage->velocity[b] + (manifold.normal * j) / _bodyStorage->mass[b];
		}
	}
}

void CollisionSolverSystem::OrientationSolver(RigidBodyComponentStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{

}

void CollisionSolverSystem::RotationSolver(RigidBodyComponentStorage* _bodyStorage, CollisionManifoldBuffer* _manifoldBuffer)
{

}
