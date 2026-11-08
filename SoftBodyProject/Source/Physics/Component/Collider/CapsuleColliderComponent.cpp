#include "PhysicsComponentAPI.h"

#include "CapsuleColliderComponent.h"

// コンストラクタ
CapsuleColliderComponent::CapsuleColliderComponent(EntityID _entity) :
	ColliderComponent{ PhysicsComponentAPI::CreateCapsule(_entity,1.0f,0.5f) }
{
}

// コンストラクタ
CapsuleColliderComponent::CapsuleColliderComponent(EntityID _entity, float _height, float _radius) :
	ColliderComponent{ PhysicsComponentAPI::CreateCapsule(_entity,_height,_radius) }
{
}

// 高さ取得
float CapsuleColliderComponent::GetHeight() const
{
	return PhysicsComponentAPI::GetCapsuleHeight(id);
}
// 高さ変更
void CapsuleColliderComponent::SetHeight(float _heibht)
{
	PhysicsComponentAPI::SetCapsuleHeight(id, _heibht);
}
// 半径取得
float CapsuleColliderComponent::GetRadius() const
{
	return PhysicsComponentAPI::GetCapsuleRadius(id);
}
// 半径変更
void CapsuleColliderComponent::SetRadius(float _radius)
{
	PhysicsComponentAPI::SetCapsuleRadius(id, _radius);
}

// オフセット位置取得
Vector3 CapsuleColliderComponent::GetOffsetPosition()
{
	return PhysicsComponentAPI::GetCapsuleOffsetPosition(id);
}
// オフセット位置変更
void CapsuleColliderComponent::SetOffsetPosition(const Vector3& _offset)
{
	PhysicsComponentAPI::SetCapsuleOffsetPosition(id, _offset);
}
// オフセット回転取得
Quaternion CapsuleColliderComponent::GetOffsetRotation()
{
	return PhysicsComponentAPI::GetCapsuleOffsetRotation(id);
}
// オフセット回転変更
void CapsuleColliderComponent::SetOffsetRotation(const Quaternion& _offset)
{
	PhysicsComponentAPI::SetCapsuleOffsetRotation(id, _offset);
}