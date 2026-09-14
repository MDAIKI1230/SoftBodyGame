#include "PhysicsComponentAPI.h"

#include "LimitedBallJointConstraintComponent.h"

// コンストラクタ
LimitedBallJointConstraintComponent::LimitedBallJointConstraintComponent(EntityID _entity) :
	ConstraintComponentBase{ PhysicsComponentAPI::CreateLimitedBallJointConstraint(_entity, Vector3::ZERO, Quaternion::Identity()) }
{
}

// Swing角度取得
float LimitedBallJointConstraintComponent::GetSwingAngle()
{
	return PhysicsComponentAPI::GetSwingAngle(id);
}
// Swing角度変更
void LimitedBallJointConstraintComponent::SetSwingAngle(float _angle)
{
	PhysicsComponentAPI::SetSwingAngle(id, _angle);
}
// Twist角度取得
float LimitedBallJointConstraintComponent::GetTwistAngle()
{
	return PhysicsComponentAPI::GetTwistAngle(id);
}
// Twist角度変更
void LimitedBallJointConstraintComponent::SetTwistAngle(float _angle)
{
	PhysicsComponentAPI::SetTwistAngle(id,_angle);
}

// 位置Tuning取得
ConstraintTuning LimitedBallJointConstraintComponent::GetPositionTuning() const
{
	return PhysicsComponentAPI::GetPositionTuning(id);
}
// 位置Tuning変更
void LimitedBallJointConstraintComponent::SetPositionTuning(const ConstraintTuning& _tuning)
{
	PhysicsComponentAPI::SetPositionTuning(id, _tuning);
}

// 回転Tuning取得
ConstraintTuning LimitedBallJointConstraintComponent::GetAngularTuning() const
{
	return PhysicsComponentAPI::GetAngularTuning(id);
}
// 回転Tuning変更
void LimitedBallJointConstraintComponent::SetAngularTuning(const ConstraintTuning& _tuning)
{
	PhysicsComponentAPI::SetAngularTuning(id, _tuning);
}