#include "PhysicsAPI.h"

#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(EntityID _entity) :
	id{ PhysicsAPI::CreateRigidBody(_entity) }
{
}

void RigidBodyComponent::AddForce(const Vector3& _force)
{
	PhysicsAPI::AddForce(id, _force);
}

void RigidBodyComponent::AddTorque(const Vector3& _torque)
{
	PhysicsAPI::AddTorque(id, _torque);
}

const Vector3& RigidBodyComponent::GetVelocity() const
{
	return PhysicsAPI::GetVelocity(id);
}

void RigidBodyComponent::SetVelocity(const Vector3& _velocity)
{
	PhysicsAPI::SetVelocity(id, _velocity);
}

const Vector3& RigidBodyComponent::GetAngularVelocity() const
{
	return PhysicsAPI::GetAngularVelocity(id);
}

void RigidBodyComponent::SetAngularVelocity(const Vector3& _omega)
{
	PhysicsAPI::SetAngularVelocity(id, _omega);
}

float RigidBodyComponent::GetMass() const
{
	return PhysicsAPI::GetMass(id);
}

void RigidBodyComponent::SetMass(float _mass)
{
	PhysicsAPI::SetMass(id, _mass);
}

bool RigidBodyComponent::GetIsGravity() const
{
	return PhysicsAPI::GetIsGravity(id);
}

void RigidBodyComponent::SetIsGravity(bool _isGravity)
{
	PhysicsAPI::SetIsGravity(id, _isGravity);
}

const Vector3& RigidBodyComponent::GetGravity() const
{
	return PhysicsAPI::GetGravity(id);
}

void RigidBodyComponent::SetGravity(const Vector3& _gravity)
{
	PhysicsAPI::SetGravity(id, _gravity);
}
