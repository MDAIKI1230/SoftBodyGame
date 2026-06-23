#include "PhysicsAPI.h"

#include "RigidBodyComponent.h"

void RigidBodyComponent::AddForce(const Vector3& _force)
{
	PhysicsAPI::AddForce(handle, _force);
}

void RigidBodyComponent::AddTorque(const Vector3& _torque)
{
	PhysicsAPI::AddTorque(handle, _torque);
}

const Vector3& RigidBodyComponent::GetVelocity() const
{
	return PhysicsAPI::GetVelocity(handle);
}

void RigidBodyComponent::SetVelocity(const Vector3& _velocity)
{
	PhysicsAPI::SetVelocity(handle, _velocity);
}

const Vector3& RigidBodyComponent::GetAngularVelocity() const
{
	return PhysicsAPI::GetAngularVelocity(handle);
}

void RigidBodyComponent::SetAngularVelocity(const Vector3& _omega)
{
	PhysicsAPI::SetAngularVelocity(handle, _omega);
}

float RigidBodyComponent::GetMass() const
{
	return PhysicsAPI::GetMass(handle);
}

void RigidBodyComponent::SetMass(float _mass)
{
	PhysicsAPI::SetMass(handle, _mass);
}

const Matrix4x4& RigidBodyComponent::GetInertiaTensor() const
{
	return PhysicsAPI::GetInertiaTensor(handle);
}

void RigidBodyComponent::SetInertiaTensor(Matrix4x4& _matrix)
{
	PhysicsAPI::SetInertiaTensor(handle, _matrix);
}

bool RigidBodyComponent::GetIsGravity() const
{
	return PhysicsAPI::GetIsGravity(handle);
}

void RigidBodyComponent::SetIsGravity(bool _isGravity)
{
	PhysicsAPI::SetIsGravity(handle, _isGravity);
}

const Vector3& RigidBodyComponent::GetGravity() const
{
	return PhysicsAPI::GetGravity(handle);
}

void RigidBodyComponent::SetGravity(const Vector3& _gravity)
{
	PhysicsAPI::SetGravity(handle, _gravity);
}
