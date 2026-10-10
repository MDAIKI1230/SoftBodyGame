#include "Base/ConstraintComponentBase.inl"

#include "PhysicsComponentAPI.h"

#include "LimitedPointOnLineConstraintComponent.h"

template struct ConstraintComponentBase<LimitedPointOnLineConstraint, false>;

// コンストラクタ
LimitedPointOnLineConstraintComponent::LimitedPointOnLineConstraintComponent(EntityID _entity) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<LimitedPointOnLineConstraint>(_entity, Vector3::ZERO, Quaternion::IDENTITY) }
{
}

// コンストラクタ
LimitedPointOnLineConstraintComponent::LimitedPointOnLineConstraintComponent(EntityID _entity, const Vector3& _localOffset) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<LimitedPointOnLineConstraint>(_entity, _localOffset, Quaternion::IDENTITY) }
{
}

// コンストラクタ
LimitedPointOnLineConstraintComponent::LimitedPointOnLineConstraintComponent(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<LimitedPointOnLineConstraint>(_entity, _localOffset, _localRotation) }
{
}

// 距離取得
float LimitedPointOnLineConstraintComponent::GetDistance() const
{
	return PhysicsComponentAPI::GetDistance(id);
}

// 距離変更
void LimitedPointOnLineConstraintComponent::SetDistance(float _distance)
{
	PhysicsComponentAPI::SetDistance(id, _distance);
}

// 単一Tuning取得
ConstraintTuning LimitedPointOnLineConstraintComponent::GetTuning() const
{
	return PhysicsComponentAPI::GetTuning(id);
}

// 単一Tuning変更
void LimitedPointOnLineConstraintComponent::SetTuning(const ConstraintTuning& _tuning)
{
	PhysicsComponentAPI::SetTuning(id, _tuning);
}
