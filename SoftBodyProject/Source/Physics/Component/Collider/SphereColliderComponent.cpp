#include "PhysicsComponentAPI.h"

#include "SphereColliderComponent.h"

SphereColliderComponent::SphereColliderComponent(EntityID _entity) :
	ColliderComponent{ PhysicsComponentAPI::CreateSphere(_entity, 1.0f) }
{
}

SphereColliderComponent::SphereColliderComponent(EntityID _entity, float _r) :
	ColliderComponent{ PhysicsComponentAPI::CreateSphere(_entity, _r) }
{
}

void SphereColliderComponent::SetRadius(float _r)
{
	PhysicsComponentAPI::SetSphereRadius(id, _r);
}

float SphereColliderComponent::GetRadius() const
{
	return PhysicsComponentAPI::GetSphereRadius(id);
}

// オフセット位置取得
Vector3 SphereColliderComponent::GetOffsetPosition()
{
	return PhysicsComponentAPI::GetSphereOffsetPosition(id);
}
// オフセット位置変更
void SphereColliderComponent::SetOffsetPosition(const Vector3& _offset)
{
	PhysicsComponentAPI::SetSphereOffsetPosition(id, _offset);
}
// オフセット回転取得
Quaternion SphereColliderComponent::GetOffsetRotation()
{
	return PhysicsComponentAPI::GetSphereOffsetRotation(id);
}
// オフセット回転変更
void SphereColliderComponent::SetOffsetRotation(const Quaternion& _offset)
{
	PhysicsComponentAPI::SetSphereOffsetRotation(id, _offset);
}