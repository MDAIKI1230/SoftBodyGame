#include "PhysicsComponentAPI.h"

#include "CharacterControllerComponent.h"

// コンストラクタ
CharacterControllerComponent::CharacterControllerComponent(EntityID _entity)
{
	id = PhysicsComponentAPI::CreateCharacterController(_entity);
}

// --- 回転 ---

// 現在の回転取得
Quaternion CharacterControllerComponent::GetRotation() const
{
	return PhysicsComponentAPI::GetCharacterControllerRotation(id);
}

// 回転設定
void CharacterControllerComponent::SetRotation(const Quaternion& _rotation)
{
	PhysicsComponentAPI::SetCharacterControllerRotation(id, _rotation);
}

// 現在の回転へ加算
void CharacterControllerComponent::Rotate(const Quaternion& _rotation)
{
	PhysicsComponentAPI::RotateCharacterController(id, _rotation);
}

// --- 操作要求 ---

// ワールド空間の移動入力取得
const Vector3& CharacterControllerComponent::GetMoveInput() const
{
	return PhysicsComponentAPI::GetCharacterControllerMoveInput(id);
}

// ワールド空間の移動入力設定
void CharacterControllerComponent::SetMoveInput(const Vector3& _moveInput)
{
	PhysicsComponentAPI::SetCharacterControllerMoveInput(id, _moveInput);
}

// 移動入力クリア
void CharacterControllerComponent::ClearMoveInput()
{
	PhysicsComponentAPI::SetCharacterControllerMoveInput(id, Vector3::ZERO);
}

// ジャンプ要求
void CharacterControllerComponent::RequestJump()
{
	PhysicsComponentAPI::SetCharacterControllerJumpRequest(id, true);
}

// 標準移動制御が有効か
bool CharacterControllerComponent::IsMotorEnabled() const
{
	return PhysicsComponentAPI::GetCharacterControllerMotorEnabled(id);
}

// 標準移動制御の有効状態設定
void CharacterControllerComponent::SetMotorEnabled(bool _enabled)
{
	PhysicsComponentAPI::SetCharacterControllerMotorEnabled(id, _enabled);
}

// --- Collider設定 ---

// カプセル中央部分の高さ取得
float CharacterControllerComponent::GetColliderHeight() const
{
	return PhysicsComponentAPI::GetCharacterControllerColliderHeight(id);
}

// カプセル中央部分の高さ設定
void CharacterControllerComponent::SetColliderHeight(float _height)
{
	PhysicsComponentAPI::SetCharacterControllerColliderHeight(id, _height);
}

// カプセル半径取得
float CharacterControllerComponent::GetColliderRadius() const
{
	return PhysicsComponentAPI::GetCharacterControllerColliderRadius(id);
}

// カプセル半径設定
void CharacterControllerComponent::SetColliderRadius(float _radius)
{
	PhysicsComponentAPI::SetCharacterControllerColliderRadius(id, _radius);
}

// --- 移動設定 ---

// 最大移動速度取得
float CharacterControllerComponent::GetMaxSpeed() const
{
	return PhysicsComponentAPI::GetCharacterControllerMaxSpeed(id);
}

// 最大移動速度設定
void CharacterControllerComponent::SetMaxSpeed(float _maxSpeed)
{
	PhysicsComponentAPI::SetCharacterControllerMaxSpeed(id, _maxSpeed);
}

// 地上での最大加速度取得
float CharacterControllerComponent::GetGroundAcceleration() const
{
	return PhysicsComponentAPI::GetCharacterControllerGroundAcceleration(id);
}

// 地上での最大加速度設定
void CharacterControllerComponent::SetGroundAcceleration(float _acceleration)
{
	PhysicsComponentAPI::SetCharacterControllerGroundAcceleration(id, _acceleration);
}

// 地上での最大減速度取得
float CharacterControllerComponent::GetGroundDeceleration() const
{
	return PhysicsComponentAPI::GetCharacterControllerGroundDeceleration(id);
}

// 地上での最大減速度設定
void CharacterControllerComponent::SetGroundDeceleration(float _deceleration)
{
	PhysicsComponentAPI::SetCharacterControllerGroundDeceleration(id, _deceleration);
}

// 空中での最大加速度取得
float CharacterControllerComponent::GetAirAcceleration() const
{
	return PhysicsComponentAPI::GetCharacterControllerAirAcceleration(id);
}

// 空中での最大加速度設定
void CharacterControllerComponent::SetAirAcceleration(float _acceleration)
{
	PhysicsComponentAPI::SetCharacterControllerAirAcceleration(id, _acceleration);
}

// ジャンプ速度取得
float CharacterControllerComponent::GetJumpSpeed() const
{
	return PhysicsComponentAPI::GetCharacterControllerJumpSpeed(id);
}

// ジャンプ速度設定
void CharacterControllerComponent::SetJumpSpeed(float _jumpSpeed)
{
	PhysicsComponentAPI::SetCharacterControllerJumpSpeed(id, _jumpSpeed);
}

// 急斜面での滑り加速度取得
float CharacterControllerComponent::GetSlopeAcceleration() const
{
	return PhysicsComponentAPI::GetCharacterControllerSlopeAcceleration(id);
}

// 急斜面での滑り加速度設定
void CharacterControllerComponent::SetSlopeAcceleration(float _acceleration)
{
	PhysicsComponentAPI::SetCharacterControllerSlopeAcceleration(id, _acceleration);
}

// --- 接地設定 ---

// 地面探索距離取得
float CharacterControllerComponent::GetGroundProbeDistance() const
{
	return PhysicsComponentAPI::GetCharacterControllerGroundProbeDistance(id);
}

// 地面探索距離設定
void CharacterControllerComponent::SetGroundProbeDistance(float _distance)
{
	PhysicsComponentAPI::SetCharacterControllerGroundProbeDistance(id, _distance);
}

// 歩行可能判定の最小法線内積取得
float CharacterControllerComponent::GetMinGroundDot() const
{
	return PhysicsComponentAPI::GetCharacterControllerMinGroundDot(id);
}

// 歩行可能判定の最小法線内積設定
void CharacterControllerComponent::SetMinGroundDot(float _minGroundDot)
{
	PhysicsComponentAPI::SetCharacterControllerMinGroundDot(id, _minGroundDot);
}

// --- 接地状態 ---

// 接地状態取得
CharacterGroundState CharacterControllerComponent::GetGroundState() const
{
	return PhysicsComponentAPI::GetCharacterControllerGroundState(id);
}

// 何らかの地面へ接触しているか
bool CharacterControllerComponent::IsGrounded() const
{
	return GetGroundState() != CharacterGroundState::AIRBORNE;
}

// 歩行可能な地面へ接触しているか
bool CharacterControllerComponent::IsOnWalkableGround() const
{
	return GetGroundState() == CharacterGroundState::WALKABLE;
}

// 急斜面へ接触しているか
bool CharacterControllerComponent::IsOnSteepSlope() const
{
	return GetGroundState() == CharacterGroundState::STEEP_SLOPE;
}

// 検出した地面法線取得
const Vector3& CharacterControllerComponent::GetGroundNormal() const
{
	return PhysicsComponentAPI::GetCharacterControllerGroundNormal(id);
}

// 検出した地面位置取得
const Vector3& CharacterControllerComponent::GetGroundPoint() const
{
	return PhysicsComponentAPI::GetCharacterControllerGroundPoint(id);
}

// カプセル底面から地面までの距離取得
float CharacterControllerComponent::GetGroundDistance() const
{
	return PhysicsComponentAPI::GetCharacterControllerGroundDistance(id);
}

// 検出した地面Collider取得
ColliderComponent CharacterControllerComponent::GetGroundCollider() const
{
	return PhysicsComponentAPI::GetCharacterControllerGroundCollider(id);
}
