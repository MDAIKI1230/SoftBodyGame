#include "PhysicsComponentAPI.h"

#include "BoxColliderComponent.h"

BoxColliderComponent::BoxColliderComponent(EntityID _entity) :
	ColliderComponent{ PhysicsComponentAPI::CreateBox(_entity, Vector3::ONE) }
{
}

BoxColliderComponent::BoxColliderComponent(EntityID _entity, float _width, float _height, float _depth) :
	ColliderComponent{ PhysicsComponentAPI::CreateBox(_entity, Vector3{ _width,_height,_depth }) }
{
}

BoxColliderComponent::BoxColliderComponent(EntityID _entity, float _size) :
	ColliderComponent{ PhysicsComponentAPI::CreateBox(_entity, Vector3{ _size }) }
{
}


float BoxColliderComponent::GetWidth() const { return PhysicsComponentAPI::GetBoxWidth(id); }

void BoxColliderComponent::SetWidth(float _width) { PhysicsComponentAPI::SetBoxWidth(id, _width); }

float BoxColliderComponent::GetHeight() const { return PhysicsComponentAPI::GetBoxHeight(id); }

void BoxColliderComponent::SetHeight(float _heibht) { PhysicsComponentAPI::SetBoxHeight(id, _heibht); }

float BoxColliderComponent::GetDepth() const { return PhysicsComponentAPI::GetBoxDepth(id); }

void BoxColliderComponent::SetDepth(float _depth) { PhysicsComponentAPI::SetBoxDepth(id, _depth); }

// オフセット位置取得
Vector3 BoxColliderComponent::GetOffsetPosition() { return PhysicsComponentAPI::GetBoxOffsetPosition(id); }
// オフセット位置変更
void BoxColliderComponent::SetOffsetPosition(const Vector3& _offset) { PhysicsComponentAPI::SetBoxOffsetPosition(id, _offset); }
// オフセット回転取得
Quaternion BoxColliderComponent::GetOffsetRotation() { return PhysicsComponentAPI::GetBoxOffsetRotation(id); }
// オフセット回転変更
void BoxColliderComponent::SetOffsetRotation(const Quaternion& _offset) { PhysicsComponentAPI::SetBoxOffsetRotation(id, _offset); }
