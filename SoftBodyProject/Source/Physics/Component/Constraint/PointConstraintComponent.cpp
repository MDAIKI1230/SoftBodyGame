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

// Bodyを指定して点拘束を作成
PointConstraintComponent::PointConstraintComponent(EntityID _entity, const RigidBodyComponent& _body, const Vector3& _localOffset) :
	ConstraintComponentBase{ PhysicsComponentAPI::CreatePointConstraint(_entity, _body.GetID(), _localOffset) }
{
}