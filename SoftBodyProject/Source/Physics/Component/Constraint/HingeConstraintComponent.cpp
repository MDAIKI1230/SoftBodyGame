#include "PhysicsComponentAPI.h"

#include "HingeConstraintComponent.h"

// コンストラクタ
HingeConstraintComponent::HingeConstraintComponent(EntityID _entity) :
	DirectionEndPointConstraintComponentBase{ PhysicsComponentAPI::CreateHingeConstraint(_entity, Vector3::ZERO, Vector3::UP) }
{
}

// コンストラクタ
HingeConstraintComponent::HingeConstraintComponent(EntityID _entity, Vector3 _localOffset) :
	DirectionEndPointConstraintComponentBase{ PhysicsComponentAPI::CreateHingeConstraint(_entity, _localOffset, Vector3::UP) }
{
}

// コンストラクタ
HingeConstraintComponent::HingeConstraintComponent(EntityID _entity, Vector3 _localOffset, Vector3 _localDirection) :
	DirectionEndPointConstraintComponentBase{ PhysicsComponentAPI::CreateHingeConstraint(_entity, _localOffset, _localDirection) }
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