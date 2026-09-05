#include "PhysicsComponentAPI.h"

#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(EntityID _entity) :
	id{ PhysicsComponentAPI::CreateRigidBody(_entity) }
{
}

// 力加算
void RigidBodyComponent::AddForce(const Vector3& _force)
{
	PhysicsComponentAPI::AddForce(id, _force);
}
// トルク加算
void RigidBodyComponent::AddTorque(const Vector3& _torque)
{
	PhysicsComponentAPI::AddTorque(id, _torque);
}

// 速度取得
const Vector3& RigidBodyComponent::GetVelocity() const
{
	return PhysicsComponentAPI::GetVelocity(id);
}
// 速度変更
void RigidBodyComponent::SetVelocity(const Vector3& _velocity)
{
	PhysicsComponentAPI::SetVelocity(id, _velocity);
}

// 角速度取得
const Vector3& RigidBodyComponent::GetAngularVelocity() const
{
	return PhysicsComponentAPI::GetAngularVelocity(id);
}
// 角速度変更
void RigidBodyComponent::SetAngularVelocity(const Vector3& _omega)
{
	PhysicsComponentAPI::SetAngularVelocity(id, _omega);
}

// 質量取得
float RigidBodyComponent::GetMass() const
{
	return PhysicsComponentAPI::GetMass(id);
}
// 質量変更
void RigidBodyComponent::SetMass(float _mass)
{
	PhysicsComponentAPI::SetMass(id, _mass);
}

// isGravity取得
bool RigidBodyComponent::GetIsGravity() const
{
	return PhysicsComponentAPI::GetIsGravity(id);
}
// isGravity変更
void RigidBodyComponent::SetIsGravity(bool _isGravity)
{
	PhysicsComponentAPI::SetIsGravity(id, _isGravity);
}

// 重力加速度取得
const Vector3& RigidBodyComponent::GetGravity() const
{
	return PhysicsComponentAPI::GetGravity(id);
}
// 重力加速度変更
void RigidBodyComponent::SetGravity(const Vector3& _gravity)
{
	PhysicsComponentAPI::SetGravity(id, _gravity);
}

// 回転制限取得
RigidBodyRotationLock RigidBodyComponent::GetRotationLock() const
{
	return PhysicsComponentAPI::GetRotationLock(id);
}
// 回転制限変更
void RigidBodyComponent::SetRotationLock(RigidBodyRotationLock _lock)
{
	PhysicsComponentAPI::SetRotationLock(id, _lock);
}
// 移動制限取得
RigidBodyPositionLock RigidBodyComponent::GetPositionLock() const
{
	return PhysicsComponentAPI::GetPositionLock(id);
}
// 移動制限変更
void RigidBodyComponent::SetPositionLock(RigidBodyPositionLock _lock)
{
	PhysicsComponentAPI::SetPositionLock(id, _lock);
}
