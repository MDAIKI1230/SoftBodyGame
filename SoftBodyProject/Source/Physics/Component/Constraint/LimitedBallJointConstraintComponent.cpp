#include "PhysicsComponentAPI.h"

#include "LimitedBallJointConstraintComponent.h"

// コンストラクタ
LimitedBallJointConstraintComponent::LimitedBallJointConstraintComponent(EntityID _entity) :
	ConstraintComponentBase{ PhysicsComponentAPI::CreateLimitedBallJointConstraint(_entity, Vector3::ZERO, Quaternion::IDENTITY) }
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

// Twist最大角度取得
float LimitedBallJointConstraintComponent::GetTwistAngleMax()
{
	return PhysicsComponentAPI::GetTwistAngleMax(id);
}
// Twist最大角度変更
void LimitedBallJointConstraintComponent::SetTwistAngleMax(float _angle)
{
	PhysicsComponentAPI::SetTwistAngleMax(id,_angle);
}
// Twist最小角度取得
float LimitedBallJointConstraintComponent::GetTwistAngleMin()
{
	return PhysicsComponentAPI::GetTwistAngleMin(id);
}
// Twist最小角度変更
void LimitedBallJointConstraintComponent::SetTwistAngleMin(float _angle)
{
	PhysicsComponentAPI::SetTwistAngleMin(id,_angle);
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