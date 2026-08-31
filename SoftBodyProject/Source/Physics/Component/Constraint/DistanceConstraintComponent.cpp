#include "PhysicsComponentAPI.h"

#include "DistanceConstraintComponent.h"


DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity) :
	EndPointConstraintComponentBase{ PhysicsComponentAPI::CreateDistanceConstraint(_entity, Vector3::ZERO, 0.0f) }
{
}

DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity, Vector3 _localOffset) :
	EndPointConstraintComponentBase{ PhysicsComponentAPI::CreateDistanceConstraint(_entity, _localOffset, 0.0f) }
{
}

DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity, float _distance) :
	EndPointConstraintComponentBase{ PhysicsComponentAPI::CreateDistanceConstraint(_entity, Vector3::ZERO, _distance) }
{
}

DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity, Vector3 _localOffset, float _distance) :
	EndPointConstraintComponentBase{ PhysicsComponentAPI::CreateDistanceConstraint(_entity, _localOffset, _distance) }
{
}

float DistanceConstraintComponent::GetDistance()
{
	return PhysicsComponentAPI::GetDistance(id);
}

void DistanceConstraintComponent::SetDistance(float _distance)
{
	PhysicsComponentAPI::SetDistance(id, _distance);
}
