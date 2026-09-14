#include "PhysicsComponentAPI.h"

#include "HingeConstraintComponent.h"

// コンストラクタ
HingeConstraintComponent::HingeConstraintComponent(EntityID _entity) :
	ConstraintComponentBase{ PhysicsComponentAPI::CreateHingeConstraint(_entity, Vector3::ZERO, Quaternion::Identity()) }
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