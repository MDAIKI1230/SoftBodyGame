#include "Base/ConstraintComponentBase.inl"

#include "PhysicsComponentAPI.h"

#include "DistanceConstraintComponent.h"

template struct ConstraintComponentBase<DistanceConstraint, false>;


DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<DistanceConstraint>(_entity, Vector3::ZERO) }
{
}

DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity, Vector3 _localOffset) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<DistanceConstraint>(_entity, _localOffset) }
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
