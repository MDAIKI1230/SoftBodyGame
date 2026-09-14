#include "PhysicsComponentAPI.h"

#include "DistanceConstraintComponent.h"


DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity) :
	ConstraintComponentBase{ PhysicsComponentAPI::CreateDistanceConstraint(_entity, Vector3::ZERO) }
{
}

DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity, Vector3 _localOffset) :
	ConstraintComponentBase{ PhysicsComponentAPI::CreateDistanceConstraint(_entity, _localOffset) }
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
