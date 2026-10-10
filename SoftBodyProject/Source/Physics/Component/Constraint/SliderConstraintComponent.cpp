#include "Base/ConstraintComponentBase.inl"

#include "PhysicsComponentAPI.h"

#include "SliderConstraintComponent.h"

template struct ConstraintComponentBase<SliderConstraint, false>;

// コンストラクタ
SliderConstraintComponent::SliderConstraintComponent(EntityID _entity) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<SliderConstraint>(_entity, Vector3::ZERO, Quaternion::IDENTITY) }
{
}

// コンストラクタ
SliderConstraintComponent::SliderConstraintComponent(EntityID _entity, const Vector3& _localOffset) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<SliderConstraint>(_entity, _localOffset, Quaternion::IDENTITY) }
{
}

// コンストラクタ
SliderConstraintComponent::SliderConstraintComponent(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<SliderConstraint>(_entity, _localOffset, _localRotation) }
{
}

// 単一Tuning取得
ConstraintTuning SliderConstraintComponent::GetTuning() const
{
	return PhysicsComponentAPI::GetTuning(id);
}

// 単一Tuning変更
void SliderConstraintComponent::SetTuning(const ConstraintTuning& _tuning)
{
	PhysicsComponentAPI::SetTuning(id, _tuning);
}
