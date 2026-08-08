#include "PhysicsAPI.h"

#include "BoxColliderComponent.h"

BoxColliderComponent::BoxColliderComponent(EntityID _entity) :
	ColliderComponent{ PhysicsAPI::CreateBox(_entity, Vector3::ONE) }
{
}

BoxColliderComponent::BoxColliderComponent(EntityID _entity, float _width, float _height, float _depth) :
	ColliderComponent{ PhysicsAPI::CreateBox(_entity, Vector3{ _width,_height,_depth }) }
{
}

BoxColliderComponent::BoxColliderComponent(EntityID _entity, float _size) :
	ColliderComponent{ PhysicsAPI::CreateBox(_entity, Vector3{ _size }) }
{
}


float BoxColliderComponent::GetWidth() const { return PhysicsAPI::GetBoxWidth(id); }

void BoxColliderComponent::SetWidth(float _width) { PhysicsAPI::SetBoxWidth(id, _width); }

float BoxColliderComponent::GetHeight() const { return PhysicsAPI::GetBoxHeight(id); }

void BoxColliderComponent::SetHeight(float _heibht) { PhysicsAPI::SetBoxHeight(id, _heibht); }

float BoxColliderComponent::GetDepth() const { return PhysicsAPI::GetBoxDepth(id); }

void BoxColliderComponent::SetDepth(float _depth) { PhysicsAPI::SetBoxDepth(id, _depth); }
