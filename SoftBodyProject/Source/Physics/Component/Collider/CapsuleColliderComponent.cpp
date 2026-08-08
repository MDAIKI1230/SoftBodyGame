#include "PhysicsAPI.h"

#include "CapsuleColliderComponent.h"

// コンストラクタ
CapsuleColliderComponent::CapsuleColliderComponent(EntityID _entity) :
	ColliderComponent{ PhysicsAPI::CreateCapsule(_entity,1.0f,0.5f) }
{
}

// コンストラクタ
CapsuleColliderComponent::CapsuleColliderComponent(EntityID _entity, float _height, float _radius) :
	ColliderComponent{ PhysicsAPI::CreateCapsule(_entity,_height,_radius) }
{
}

// 高さ取得
float CapsuleColliderComponent::GetHeight() const
{
	return PhysicsAPI::GetCapsuleHeight(id);
}
// 高さ変更
void CapsuleColliderComponent::SetHeight(float _heibht)
{
	PhysicsAPI::SetCapsuleHeight(id, _heibht);
}
// 半径取得
float CapsuleColliderComponent::GetRadius() const
{
	return PhysicsAPI::GetCapsuleRadius(id);
}
// 半径変更
void CapsuleColliderComponent::SetRadius(float _radius)
{
	PhysicsAPI::SetCapsuleRadius(id, _radius);
}
