#include "PhysicsAPI.h"

#include "PointConstraintComponent.h"

PointConstraintComponent::PointConstraintComponent(EntityID _entity) :
	EndPointConstraintComponentBase{ PhysicsAPI::CreatePointConstraint(_entity, Vector3::ZERO) }
{
}

PointConstraintComponent::PointConstraintComponent(EntityID _entity, Vector3 _localOffset):
	EndPointConstraintComponentBase{ PhysicsAPI::CreatePointConstraint(_entity, _localOffset) }
{
}
