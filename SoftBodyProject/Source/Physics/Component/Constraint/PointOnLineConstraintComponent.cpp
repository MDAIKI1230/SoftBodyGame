#include "Base/ConstraintComponentBase.inl"

#include "PhysicsComponentAPI.h"

#include "PointOnLineConstraintComponent.h"

template struct ConstraintComponentBase<PointOnLineConstraint, false>;

// コンストラクタ
PointOnLineConstraintComponent::PointOnLineConstraintComponent(EntityID _entity) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<PointOnLineConstraint>(_entity, Vector3::ZERO, Quaternion::IDENTITY) }
{
}

// コンストラクタ
PointOnLineConstraintComponent::PointOnLineConstraintComponent(EntityID _entity, const Vector3& _localOffset) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<PointOnLineConstraint>(_entity, _localOffset, Quaternion::IDENTITY) }
{
}

// コンストラクタ
PointOnLineConstraintComponent::PointOnLineConstraintComponent(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<PointOnLineConstraint>(_entity, _localOffset, _localRotation) }
{
}

// 単一Tuning取得
ConstraintTuning PointOnLineConstraintComponent::GetTuning() const
{
	return PhysicsComponentAPI::GetTuning(id);
}

// 単一Tuning変更
void PointOnLineConstraintComponent::SetTuning(const ConstraintTuning& _tuning)
{
	PhysicsComponentAPI::SetTuning(id, _tuning);
}
