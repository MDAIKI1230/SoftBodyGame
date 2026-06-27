#include "PhysicsAPI.h"

#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent() :
	ComponentBase{ -1 }
{
}

RigidBodyComponent::RigidBodyComponent(EntityID _entity, int _handle) :
	ComponentBase{ _handle }
{
	bodyId = PhysicsAPI::CreateRigidBody(_entity);
}

void RigidBodyComponent::AddForce(const Vector3& _force)
{
	PhysicsAPI::AddForce(bodyId, _force);
}

void RigidBodyComponent::AddTorque(const Vector3& _torque)
{
	PhysicsAPI::AddTorque(bodyId, _torque);
}

const Vector3& RigidBodyComponent::GetVelocity() const
{
	return PhysicsAPI::GetVelocity(bodyId);
}

void RigidBodyComponent::SetVelocity(const Vector3& _velocity)
{
	PhysicsAPI::SetVelocity(bodyId, _velocity);
}

const Vector3& RigidBodyComponent::GetAngularVelocity() const
{
	return PhysicsAPI::GetAngularVelocity(bodyId);
}

void RigidBodyComponent::SetAngularVelocity(const Vector3& _omega)
{
	PhysicsAPI::SetAngularVelocity(bodyId, _omega);
}

float RigidBodyComponent::GetMass() const
{
	return PhysicsAPI::GetMass(bodyId);
}

void RigidBodyComponent::SetMass(float _mass)
{
	PhysicsAPI::SetMass(bodyId, _mass);
}

const Matrix4x4& RigidBodyComponent::GetInertiaTensor() const
{
	return PhysicsAPI::GetInertiaTensor(bodyId);
}

void RigidBodyComponent::SetInertiaTensor(Matrix4x4& _matrix)
{
	PhysicsAPI::SetInertiaTensor(bodyId, _matrix);
}

bool RigidBodyComponent::GetIsGravity() const
{
	return PhysicsAPI::GetIsGravity(bodyId);
}

void RigidBodyComponent::SetIsGravity(bool _isGravity)
{
	PhysicsAPI::SetIsGravity(bodyId, _isGravity);
}

const Vector3& RigidBodyComponent::GetGravity() const
{
	return PhysicsAPI::GetGravity(bodyId);
}

void RigidBodyComponent::SetGravity(const Vector3& _gravity)
{
	PhysicsAPI::SetGravity(bodyId, _gravity);
}
