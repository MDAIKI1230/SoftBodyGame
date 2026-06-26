#include "PhysicsAPI.h"

#include "BoxColliderComponent.h"

BoxColliderComponent::BoxColliderComponent(int _handle, float _width, float _height, float _depth):
	ColliderComponent{_handle}
{
	PhysicsAPI::SetWidth(handle, _width);
	PhysicsAPI::SetHeight(handle, _height);
	PhysicsAPI::SetDepth(handle, _depth);
}

// X方向の長さ取得
float BoxColliderComponent::GetWidth()
{
	return PhysicsAPI::GetWidth(handle);
}

// X方向の長さ変更
void BoxColliderComponent::SetWidth(float _width)
{
	PhysicsAPI::SetWidth(handle, _width);
}

// Y方向の長さ取得
float BoxColliderComponent::GetHeight()
{
	return PhysicsAPI::GetHeight(handle);
}

// Y方向の長さ変更
void BoxColliderComponent::SetHeight(float _heibht)
{
	PhysicsAPI::SetHeight(handle, _heibht);
}

// Z方向の長さ取得
float BoxColliderComponent::GetDepth()
{
	return PhysicsAPI::GetDepth(handle);
}

// Z方向の長さ変更
void BoxColliderComponent::SetDepth(float _depth)
{
	PhysicsAPI::SetDepth(handle, _depth);
}
