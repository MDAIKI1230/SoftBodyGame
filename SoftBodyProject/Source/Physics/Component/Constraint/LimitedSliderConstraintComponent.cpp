#include "Base/ConstraintComponentBase.inl"

#include "PhysicsComponentAPI.h"

#include "LimitedSliderConstraintComponent.h"

template struct ConstraintComponentBase<LimitedSliderConstraint, false>;

// コンストラクタ
LimitedSliderConstraintComponent::LimitedSliderConstraintComponent(EntityID _entity) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<LimitedSliderConstraint>(_entity, Vector3::ZERO, Quaternion::IDENTITY) }
{
}

// コンストラクタ
LimitedSliderConstraintComponent::LimitedSliderConstraintComponent(EntityID _entity, const Vector3& _localOffset) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<LimitedSliderConstraint>(_entity, _localOffset, Quaternion::IDENTITY) }
{
}

// コンストラクタ
LimitedSliderConstraintComponent::LimitedSliderConstraintComponent(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<LimitedSliderConstraint>(_entity, _localOffset, _localRotation) }
{
}

// 距離取得
float LimitedSliderConstraintComponent::GetDistance() const
{
	return PhysicsComponentAPI::GetDistance(id);
}

// 距離変更
void LimitedSliderConstraintComponent::SetDistance(float _distance)
{
	PhysicsComponentAPI::SetDistance(id, _distance);
}

// 単一Tuning取得
ConstraintTuning LimitedSliderConstraintComponent::GetTuning() const
{
	return PhysicsComponentAPI::GetTuning(id);
}

// 単一Tuning変更
void LimitedSliderConstraintComponent::SetTuning(const ConstraintTuning& _tuning)
{
	PhysicsComponentAPI::SetTuning(id, _tuning);
}
