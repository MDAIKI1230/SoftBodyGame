#include "PhysicsAPI.h"

#include "BoxColliderComponent.h"

BoxColliderComponent::BoxColliderComponent(EntityID _entity, int _handle) :
	ColliderComponent{ _handle }
{
	PhysicsAPI::CreateBox(_entity, Vector3::ONE);
}

BoxColliderComponent::BoxColliderComponent(EntityID _entity, int _handle, float _width, float _height, float _depth) :
	ColliderComponent{ _handle }
{
	PhysicsAPI::CreateBox(_entity, Vector3{ _width,_height,_depth });
}

BoxColliderComponent::BoxColliderComponent(EntityID _entity, int _handle, float _size) :
	ColliderComponent{ _handle }
{
	PhysicsAPI::CreateBox(_entity, Vector3{ _size });
}


float BoxColliderComponent::GetWidth() { return PhysicsAPI::GetWidth(colliderID); }

void BoxColliderComponent::SetWidth(float _width) { PhysicsAPI::SetWidth(colliderID, _width); }

float BoxColliderComponent::GetHeight() { return PhysicsAPI::GetHeight(colliderID); }

void BoxColliderComponent::SetHeight(float _heibht) { PhysicsAPI::SetHeight(colliderID, _heibht); }

float BoxColliderComponent::GetDepth() { return PhysicsAPI::GetDepth(colliderID); }

void BoxColliderComponent::SetDepth(float _depth) { PhysicsAPI::SetDepth(colliderID, _depth); }
