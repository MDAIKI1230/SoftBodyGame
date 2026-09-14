#include "PhysicsComponentAPI.h"

#include "AngleLimitHingeConstraintComponent.h"

// コンストラクタ
AngleLimitHingeConstraintComponent::AngleLimitHingeConstraintComponent(EntityID _entity) :
	ConstraintComponentBase{ PhysicsComponentAPI::CreateAngleLimitHingeConstraint(_entity, Vector3::ZERO, Quaternion::Identity()) }
{
}

// コンストラクタ
AngleLimitHingeConstraintComponent::AngleLimitHingeConstraintComponent(EntityID _entity, const Vector3& _localOffset) :
	ConstraintComponentBase{ PhysicsComponentAPI::CreateAngleLimitHingeConstraint(_entity, _localOffset, Quaternion::Identity()) }
{
}

// コンストラクタ
AngleLimitHingeConstraintComponent::AngleLimitHingeConstraintComponent(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation) :
	ConstraintComponentBase{ PhysicsComponentAPI::CreateAngleLimitHingeConstraint(_entity, _localOffset, _localRotation) }
{
}

// 最小角度取得
float AngleLimitHingeConstraintComponent::GetAngleMin() const
{
	return PhysicsComponentAPI::GetAngleMin(id);
}

// 最小角度変更
void AngleLimitHingeConstraintComponent::SetAngleMin(float _angleMin)
{
	PhysicsComponentAPI::SetAngleMin(id, _angleMin);
}

// 最大角度取得
float AngleLimitHingeConstraintComponent::GetAngleMax() const
{
	return PhysicsComponentAPI::GetAngleMax(id);
}

// 最大角度変更
void AngleLimitHingeConstraintComponent::SetAngleMax(float _angleMax)
{
	PhysicsComponentAPI::SetAngleMax(id, _angleMax);
}

// 角度範囲変更
void AngleLimitHingeConstraintComponent::SetAngleRange(float _angleMin, float _angleMax)
{
	PhysicsComponentAPI::SetAngleRange(id, _angleMin, _angleMax);
}

// 位置Tuning取得
ConstraintTuning AngleLimitHingeConstraintComponent::GetPositionTuning() const
{
	return PhysicsComponentAPI::GetPositionTuning(id);
}

// 位置Tuning変更
void AngleLimitHingeConstraintComponent::SetPositionTuning(const ConstraintTuning& _tuning)
{
	PhysicsComponentAPI::SetPositionTuning(id, _tuning);
}

// 回転Tuning取得
ConstraintTuning AngleLimitHingeConstraintComponent::GetAngularTuning() const
{
	return PhysicsComponentAPI::GetAngularTuning(id);
}

// 回転Tuning変更
void AngleLimitHingeConstraintComponent::SetAngularTuning(const ConstraintTuning& _tuning)
{
	PhysicsComponentAPI::SetAngularTuning(id, _tuning);
}