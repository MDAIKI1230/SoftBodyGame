#include "Base/ConstraintComponentBase.inl"

#include "PhysicsComponentAPI.h"

#include "PointConstraintComponent.h"

template struct ConstraintComponentBase<PointConstraint, false>;

PointConstraintComponent::PointConstraintComponent(EntityID _entity) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<PointConstraint>(_entity, Vector3::ZERO) }
{
}

PointConstraintComponent::PointConstraintComponent(EntityID _entity, Vector3 _localOffset):
	ConstraintComponentBase{ PhysicsComponentAPI::Create<PointConstraint>(_entity, _localOffset) }
{
}

// Bodyを指定して点拘束を作成
PointConstraintComponent::PointConstraintComponent(EntityID _entity, const RigidBodyComponent& _body, const Vector3& _localOffset) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<PointConstraint>(_entity, _body.GetID(), _localOffset) }
{
}