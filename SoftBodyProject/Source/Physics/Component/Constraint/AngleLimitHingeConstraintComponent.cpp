#include "PhysicsComponentAPI.h"

#include "AngleLimitHingeConstraintComponent.h"

// コンストラクタ
AngleLimitHingeConstraintComponent::AngleLimitHingeConstraintComponent(EntityID _entity) :
	id{ PhysicsComponentAPI::CreateAngleLimitHingeConstraint(_entity, Vector3::ZERO, Vector3::UP, Vector3::FORWARD, 0.0f, 0.0f) }
{
}

// コンストラクタ
AngleLimitHingeConstraintComponent::AngleLimitHingeConstraintComponent(EntityID _entity, Vector3 _localOffset) :
	id{ PhysicsComponentAPI::CreateAngleLimitHingeConstraint(_entity, _localOffset, Vector3::UP, Vector3::FORWARD, 0.0f, 0.0f) }
{
}

// コンストラクタ
AngleLimitHingeConstraintComponent::AngleLimitHingeConstraintComponent(EntityID _entity, float _angleMin, float _angleMax) :
	id{ PhysicsComponentAPI::CreateAngleLimitHingeConstraint(_entity, Vector3::ZERO, Vector3::UP, Vector3::FORWARD, _angleMin, _angleMax) }
{
}

// コンストラクタ
AngleLimitHingeConstraintComponent::AngleLimitHingeConstraintComponent(EntityID _entity, Vector3 _localOffset, Vector3 _localAxis, Vector3 _localDirection) :
	id{ PhysicsComponentAPI::CreateAngleLimitHingeConstraint(_entity, _localOffset, _localAxis, _localDirection, 0.0f, 0.0f) }
{
}

// コンストラクタ
AngleLimitHingeConstraintComponent::AngleLimitHingeConstraintComponent(EntityID _entity, Vector3 _localOffset, Vector3 _localAxis, Vector3 _localDirection, float _angleMin, float _angleMax) :
	id{ PhysicsComponentAPI::CreateAngleLimitHingeConstraint(_entity, _localOffset, _localAxis, _localDirection, _angleMin, _angleMax) }
{
}

// 対応点追加
void AngleLimitHingeConstraintComponent::AddEndPoint(EntityID _entityID, const Vector3& _localOffset, const Vector3& _localAxis, const Vector3& _localDirection)
{
	PhysicsComponentAPI::AddAngleLimitHingeEndPoint(id, _entityID, _localOffset, _localAxis, _localDirection);
}

// 対応点削除
void AngleLimitHingeConstraintComponent::RemoveEndPoint(EntityID _entityID)
{
	PhysicsComponentAPI::RemoveEndPoint(id, _entityID);
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