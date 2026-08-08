#include "PhysicsAPI.h"

#include "SphereColliderComponent.h"

SphereColliderComponent::SphereColliderComponent(EntityID _entity) :
	ColliderComponent{ PhysicsAPI::CreateSphere(_entity, 1.0f) }
{
}

SphereColliderComponent::SphereColliderComponent(EntityID _entity, float _r) :
	ColliderComponent{ PhysicsAPI::CreateSphere(_entity, _r) }
{
}

void SphereColliderComponent::SetRadius(float _r)
{
	PhysicsAPI::SetSphereRadius(id, _r);
}

float SphereColliderComponent::GetRadius() const
{
	return PhysicsAPI::GetSphereRadius(id);
}
