#include "Base/ConstraintComponentBase.inl"

#include "PhysicsComponentAPI.h"

#include "HingeConstraintComponent.h"

template struct ConstraintComponentBase<HingeConstraint, false>;

// コンストラクタ
HingeConstraintComponent::HingeConstraintComponent(EntityID _entity) :
	ConstraintComponentBase{ PhysicsComponentAPI::Create<HingeConstraint>(_entity, Vector3::ZERO, Quaternion::IDENTITY) }
{
}

// 位置Tuning取得
ConstraintTuning HingeConstraintComponent::GetPositionTuning() const
{
	return PhysicsComponentAPI::GetPositionTuning(id);
}

// 位置Tuning変更
void HingeConstraintComponent::SetPositionTuning(const ConstraintTuning& _tuning)
{
	PhysicsComponentAPI::SetPositionTuning(id, _tuning);
}

// 回転Tuning取得
ConstraintTuning HingeConstraintComponent::GetAngularTuning() const
{
	return PhysicsComponentAPI::GetAngularTuning(id);
}

// 回転Tuning変更
void HingeConstraintComponent::SetAngularTuning(const ConstraintTuning& _tuning)
{
	PhysicsComponentAPI::SetAngularTuning(id, _tuning);
}