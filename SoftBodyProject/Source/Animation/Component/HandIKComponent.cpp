#include "AnimationComponentAPI.h"

#include "HandIKComponent.h"

// コンストラクタ
HandIKComponent::HandIKComponent(EntityID _entity, const RendererComponent* _renderer) :
	id{ AnimationComponentAPI::CreateHandIK(_entity, _renderer) }
{
}

// 上腕ボーン取得
std::string_view HandIKComponent::GetUpperArm()
{
	return AnimationComponentAPI::GetUpperArm(id);
}

// 上腕ボーン変更
void HandIKComponent::SetUpperArm(const char* _boneName)
{
	AnimationComponentAPI::SetUpperArm(id, _boneName);
}

// 前腕ボーン取得
std::string_view HandIKComponent::GetLowerArm()
{
	return AnimationComponentAPI::GetLowerArm(id);
}

// 前腕ボーン変更
void HandIKComponent::SetLowerArm(const char* _boneName)
{
	AnimationComponentAPI::SetLowerArm(id, _boneName);
}

// 手ボーン取得
std::string_view HandIKComponent::GetHand()
{
	return AnimationComponentAPI::GetHand(id);
}

// 手ボーン変更
void HandIKComponent::SetHand(const char* _boneName)
{
	AnimationComponentAPI::SetHand(id, _boneName);
}

// ターゲット位置取得
Vector3 HandIKComponent::GetTargetPosition()
{
	return AnimationComponentAPI::GetTargetPosition(id);
}

// ターゲット位置変更
void HandIKComponent::SetTargetPosition(const Vector3& _targetPosition)
{
	AnimationComponentAPI::SetTargetPosition(id, _targetPosition);
}

// ターゲット回転取得
Quaternion HandIKComponent::GetTargetRotation()
{
	return AnimationComponentAPI::GetTargetRotation(id);
}

// ターゲット回転変更
void HandIKComponent::SetTargetRotation(const Quaternion& _targetRotation)
{
	AnimationComponentAPI::SetTargetRotation(id, _targetRotation);
}

// ポール位置取得
Vector3 HandIKComponent::GetPolePosition()
{
	return AnimationComponentAPI::GetPolePosition(id);
}

// ポール位置変更
void HandIKComponent::SetPolePosition(const Vector3& _polePosition)
{
	AnimationComponentAPI::SetPolePosition(id, _polePosition);
}

// 位置ウェイト取得
float HandIKComponent::GetPositionWeight()
{
	return AnimationComponentAPI::GetPositionWeight(id);
}

// 位置ウェイト変更
void HandIKComponent::SetPositionWeight(float _positionWeight)
{
	AnimationComponentAPI::SetPositionWeight(id, _positionWeight);
}

// 回転ウェイト取得
float HandIKComponent::GetRotationWeight()
{
	return AnimationComponentAPI::GetRotationWeight(id);
}

// 回転ウェイト変更
void HandIKComponent::SetRotationWeight(float _rotationWeight)
{
	AnimationComponentAPI::SetRotationWeight(id, _rotationWeight);
}