#include "PhysicsAPI.h"

#include "SphereColliderComponent.h"

SphereColliderComponent::SphereColliderComponent(EntityID _entity, int _handle) :
	ColliderComponent{ _handle }
{
	colliderID = PhysicsAPI::CreateSphere(_entity, 1.0f);
}

SphereColliderComponent::SphereColliderComponent(EntityID _entity, int _handle, float _r) :
	ColliderComponent{ _handle },
	r{ _r }
{
	colliderID = PhysicsAPI::CreateSphere(_entity, _r);
}

void SphereColliderComponent::SetRadius(float _r)
{
	PhysicsAPI::SetRadius(colliderID, _r);
}

float SphereColliderComponent::GetRadius()
{
	return PhysicsAPI::GetRadius(colliderID);
}
