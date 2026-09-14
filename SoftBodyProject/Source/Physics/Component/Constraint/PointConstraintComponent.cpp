#include "PhysicsComponentAPI.h"

#include "PointConstraintComponent.h"

PointConstraintComponent::PointConstraintComponent(EntityID _entity) :
	ConstraintComponentBase{ PhysicsComponentAPI::CreatePointConstraint(_entity, Vector3::ZERO) }
{
}

PointConstraintComponent::PointConstraintComponent(EntityID _entity, Vector3 _localOffset):
	ConstraintComponentBase{ PhysicsComponentAPI::CreatePointConstraint(_entity, _localOffset) }
{
}
