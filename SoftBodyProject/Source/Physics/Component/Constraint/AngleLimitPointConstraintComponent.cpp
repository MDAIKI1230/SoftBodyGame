#include "PhysicsComponentAPI.h"

#include "AngleLimitPointConstraintComponent.h"

// コンストラクタ
AngleLimitPointConstraintComponent::AngleLimitPointConstraintComponent(EntityID _entity) :
	DirectionEndPointConstraintComponentBase{ PhysicsComponentAPI::CreateAngleLimitPointConstraint(_entity, Vector3::ZERO, Vector3::UP, 0.0f, 0.0f) }
{
}

// コンストラクタ
AngleLimitPointConstraintComponent::AngleLimitPointConstraintComponent(EntityID _entity, Vector3 _localOffset) :
	DirectionEndPointConstraintComponentBase{ PhysicsComponentAPI::CreateAngleLimitPointConstraint(_entity, _localOffset, Vector3::UP, 0.0f, 0.0f) }
{
}

// コンストラクタ
AngleLimitPointConstraintComponent::AngleLimitPointConstraintComponent(EntityID _entity, float _angleMin, float _angleMax) :
	DirectionEndPointConstraintComponentBase{ PhysicsComponentAPI::CreateAngleLimitPointConstraint(_entity, Vector3::ZERO, Vector3::UP, _angleMin, _angleMax) }
{
}

// コンストラクタ
AngleLimitPointConstraintComponent::AngleLimitPointConstraintComponent(EntityID _entity, Vector3 _localOffset, Vector3 _localDirection) :
	DirectionEndPointConstraintComponentBase{ PhysicsComponentAPI::CreateAngleLimitPointConstraint(_entity, _localOffset, _localDirection, 0.0f, 0.0f) }
{
}

// コンストラクタ
AngleLimitPointConstraintComponent::AngleLimitPointConstraintComponent(EntityID _entity, Vector3 _localOffset, Vector3 _localDirection, float _angleMin, float _angleMax) :
	DirectionEndPointConstraintComponentBase{ PhysicsComponentAPI::CreateAngleLimitPointConstraint(_entity, _localOffset, _localDirection, _angleMin, _angleMax) }
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