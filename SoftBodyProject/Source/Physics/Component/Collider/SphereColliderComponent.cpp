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
