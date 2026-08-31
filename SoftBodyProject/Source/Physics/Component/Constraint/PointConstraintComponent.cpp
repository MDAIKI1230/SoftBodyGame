#include "PhysicsComponentAPI.h"

#include "PointConstraintComponent.h"

PointConstraintComponent::PointConstraintComponent(EntityID _entity) :
	EndPointConstraintComponentBase{ PhysicsComponentAPI::CreatePointConstraint(_entity, Vector3::ZERO) }
{
}

PointConstraintComponent::PointConstraintComponent(EntityID _entity, Vector3 _localOffset):
	EndPointConstraintComponentBase{ PhysicsComponentAPI::CreatePointConstraint(_entity, _localOffset) }
{
}
