#include "NamericalAnalysis.h"

#include "RigidBodyComponentStorage.h"
#include "BoxColliderComponentStorage.h"

#include "PhysicsAPI.h"

BodyID PhysicsAPI::CreateRigidBody(EntityID _entity)
{
	return rigidBodyStorage->CreateRigidBody(_entity, transformStorage->GetOrCreateTransform(_entity));
}

void PhysicsAPI::AddForce(BodyID _id, const Vector3& _force)
{
	// 加算
	rigidBodyStorage->force[rigidBodyStorage->GetDenseIndex(_id)] += _force;
}
void PhysicsAPI::AddTorque(BodyID _id, const Vector3& _torque)
{
	// 加算
	rigidBodyStorage->torque[rigidBodyStorage->GetDenseIndex(_id)] += _torque;
}

const Vector3& PhysicsAPI::GetVelocity(BodyID _id)
{
	return rigidBodyStorage->velocity[rigidBodyStorage->GetDenseIndex(_id)];
}

void PhysicsAPI::SetVelocity(BodyID _id, const Vector3& _velocity)
{
	rigidBodyStorage->velocity[rigidBodyStorage->GetDenseIndex(_id)] = _velocity;
}

const Vector3& PhysicsAPI::GetAngularVelocity(BodyID _id)
{
	return rigidBodyStorage->angularVelocity[rigidBodyStorage->GetDenseIndex(_id)];
}

void PhysicsAPI::SetAngularVelocity(BodyID _id, const Vector3& _omega)
{
	rigidBodyStorage->angularVelocity[rigidBodyStorage->GetDenseIndex(_id)] = _omega;
}

float PhysicsAPI::GetMass(BodyID _id)
{
	return rigidBodyStorage->mass[rigidBodyStorage->GetDenseIndex(_id)];
}

void PhysicsAPI::SetMass(BodyID _id, float _mass)
{
	if (_mass <= 0)
	{
		return;
	}

	rigidBodyStorage->mass[rigidBodyStorage->GetDenseIndex(_id)] = _mass;
	rigidBodyStorage->inverseMass[rigidBodyStorage->GetDenseIndex(_id)] = 1 / _mass;
	rigidBodyStorage->localInertiaDirty[rigidBodyStorage->GetDenseIndex(_id)] = true;
}

const Matrix4x4& PhysicsAPI::GetInertiaTensor(BodyID _id)
{
	return rigidBodyStorage->inertiaTensor[rigidBodyStorage->GetDenseIndex(_id)];
}

void PhysicsAPI::SetInertiaTensor(BodyID _id, Matrix4x4& _matrix)
{
	rigidBodyStorage->inertiaTensor[rigidBodyStorage->GetDenseIndex(_id)] = _matrix;
	rigidBodyStorage->localInverseInertiaTensor[rigidBodyStorage->GetDenseIndex(_id)] = NamericalAnalysis::GaussJordan(_matrix);
	rigidBodyStorage->localInertiaDirty[rigidBodyStorage->GetDenseIndex(_id)] = true;
}

bool PhysicsAPI::GetIsGravity(BodyID _id)
{
	return rigidBodyStorage->isGravity[rigidBodyStorage->GetDenseIndex(_id)];
}

void PhysicsAPI::SetIsGravity(BodyID _id, bool _isGravity)
{
	rigidBodyStorage->isGravity[rigidBodyStorage->GetDenseIndex(_id)] = _isGravity;
}

const Vector3& PhysicsAPI::GetGravity(BodyID _id)
{
	return rigidBodyStorage->gravity[rigidBodyStorage->GetDenseIndex(_id)];
}

void PhysicsAPI::SetGravity(BodyID _id, const Vector3& _gravity)
{
	rigidBodyStorage->gravity[rigidBodyStorage->GetDenseIndex(_id)] = _gravity;
}

ColliderID PhysicsAPI::CreateSphere(EntityID _entity, float _radius)
{
	return colliderStorage->CreateSphere(_entity, transformStorage->GetOrCreateTransform(_entity), _radius);
}

ColliderID PhysicsAPI::CreateBox(EntityID _entity, const Vector3& _scale)
{
	return colliderStorage->CreateBox(_entity, transformStorage->GetOrCreateTransform(_entity), _scale);
}

float PhysicsAPI::GetRadius(ColliderID _id)
{
	return colliderStorage->sphereStorage->radius[colliderStorage->GetDenseIndex(_id)];
}

void PhysicsAPI::SetRadius(ColliderID _id, float _radius)
{
	colliderStorage->sphereStorage->radius[colliderStorage->GetDenseIndex(_id)] = _radius;
}

float PhysicsAPI::GetWidth(ColliderID _id)
{
	return colliderStorage->boxStorage->scale[colliderStorage->GetDenseIndex(_id)].x;
}

void PhysicsAPI::SetWidth(ColliderID _id, float _width)
{
	colliderStorage->boxStorage->scale[colliderStorage->GetDenseIndex(_id)].x = _width;
}

float PhysicsAPI::GetHeight(ColliderID _id)
{
	return colliderStorage->boxStorage->scale[colliderStorage->GetDenseIndex(_id)].y;
}

void PhysicsAPI::SetHeight(ColliderID _id, float _height)
{
	colliderStorage->boxStorage->scale[colliderStorage->GetDenseIndex(_id)].y = _height;
}

float PhysicsAPI::GetDepth(ColliderID _id)
{
	return colliderStorage->boxStorage->scale[colliderStorage->GetDenseIndex(_id)].z;
}

void PhysicsAPI::SetDepth(ColliderID _id, float _depth)
{
	colliderStorage->boxStorage->scale[colliderStorage->GetDenseIndex(_id)].z = _depth;
}

ConstraintID PhysicsAPI::CreatePointConstraint(EntityID& _entity, const Vector3& _localOffset)
{
	return constraintStorage->CreatePointConstraint(_entity, transformStorage->GetOrCreateTransform(_entity), _localOffset);
}

void PhysicsAPI::AddEndPoint(ConstraintID& _id, EntityID& _entity, Vector3& _localOffset)
{
	PhysicsTransformID transformID;
	// エンティティに対応したTransformがあるならそれを追加ないなら何もしない
	if (transformStorage->TryGet(_entity, transformID))
	{
		uint32_t index{ constraintStorage->GetDenseIndex(_id) };
		constraintStorage->pointConstraintStorage->constraints[index].endPoints.emplace_back(transformID, _localOffset);
	}
}

void  PhysicsAPI::RemoveEndPoint(ConstraintID& _id, EntityID& _entity)
{
	PhysicsTransformID transformID;
	// エンティティに対応したTransformがあるならそれを除外ないなら何もしない
	if (transformStorage->TryGet(_entity, transformID))
	{
		uint32_t index{ constraintStorage->GetDenseIndex(_id) };
		constraintStorage->pointConstraintStorage->constraints[index].RemoveEndpoint(transformID);
	}
}

PhysicsWorld* PhysicsAPI::physicsWorld{ nullptr };
ColliderStorage* PhysicsAPI::colliderStorage{ nullptr };
RigidBodyStorage* PhysicsAPI::rigidBodyStorage{ nullptr };
PhysicsTransformStorage* PhysicsAPI::transformStorage{ nullptr };
ConstraintStorage* PhysicsAPI::constraintStorage{ nullptr };
