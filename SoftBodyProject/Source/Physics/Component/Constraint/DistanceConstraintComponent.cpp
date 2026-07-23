#include "PhysicsAPI.h"

#include "DistanceConstraintComponent.h"


DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity) :
	id{ PhysicsAPI::CreateDistanceConstraint(_entity, Vector3::ZERO, 0.0f) }
{
}

DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity, Vector3 _localOffset) :
	id{ PhysicsAPI::CreateDistanceConstraint(_entity, _localOffset, 0.0f) }
{
}

DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity, float _distance) :
	id{ PhysicsAPI::CreateDistanceConstraint(_entity, Vector3::ZERO, _distance) }
{
}

DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity, Vector3 _localOffset, float _distance) :
	id{ PhysicsAPI::CreateDistanceConstraint(_entity, _localOffset, _distance) }
{
}

void DistanceConstraintComponent::AddEndPoint(const EntityID& _entityID, const Vector3& _localOffset)
{
	PhysicsAPI::AddEndPoint(id, _entityID, _localOffset);
}

float DistanceConstraintComponent::GetDistance()
{
	return PhysicsAPI::GetDistance(id);
}

void DistanceConstraintComponent::SetDistance(float _distance)
{
	PhysicsAPI::SetDistance(id, _distance);
}
