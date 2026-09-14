#include "PhysicsComponentAPI.h"

#include "AngleLimitPointConstraintComponent.h"

// コンストラクタ
AngleLimitPointConstraintComponent::AngleLimitPointConstraintComponent(EntityID _entity) :
	ConstraintComponentBase{ PhysicsComponentAPI::CreateAngleLimitPointConstraint(_entity, Vector3::ZERO, Quaternion::Identity()) }
{
}

// コンストラクタ
AngleLimitPointConstraintComponent::AngleLimitPointConstraintComponent(EntityID _entity, const Vector3& _localOffset) :
	ConstraintComponentBase{ PhysicsComponentAPI::CreateAngleLimitPointConstraint(_entity, _localOffset, Quaternion::Identity()) }
{
}

// コンストラクタ
AngleLimitPointConstraintComponent::AngleLimitPointConstraintComponent(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation) :
	ConstraintComponentBase{ PhysicsComponentAPI::CreateAngleLimitPointConstraint(_entity, _localOffset, _localRotation) }
{
}

// 最小角度取得
float AngleLimitPointConstraintComponent::GetAngleMin() const
{
	return PhysicsComponentAPI::GetAngleMin(id);
}

// 最小角度変更
void AngleLimitPointConstraintComponent::SetAngleMin(float _angleMin)
{
	PhysicsComponentAPI::SetAngleMin(id, _angleMin);
}

// 最大角度取得
float AngleLimitPointConstraintComponent::GetAngleMax() const
{
	return PhysicsComponentAPI::GetAngleMax(id);
}

// 最大角度変更
void AngleLimitPointConstraintComponent::SetAngleMax(float _angleMax)
{
	PhysicsComponentAPI::SetAngleMax(id, _angleMax);
}

// 角度範囲変更
void AngleLimitPointConstraintComponent::SetAngleRange(float _angleMin, float _angleMax)
{
	PhysicsComponentAPI::SetAngleRange(id, _angleMin, _angleMax);
}

// Tuning取得
ConstraintTuning AngleLimitPointConstraintComponent::GetTuning() const
{
	return PhysicsComponentAPI::GetTuning(id);
}

// Tuning変更
void AngleLimitPointConstraintComponent::SetTuning(const ConstraintTuning& _tuning)
{
	PhysicsComponentAPI::SetTuning(id, _tuning);
}