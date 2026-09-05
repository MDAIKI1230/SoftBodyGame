#pragma once

#include "MDMath.h"

#include "EntityID.h"
#include "CharacterControllerID.h"

#include "CharacterControllerConstants.h"

#include "ColliderComponent.h"

struct CharacterControllerComponent
{
public:
	// コンストラクタ
	CharacterControllerComponent(EntityID _entity);

	// --- 回転 ---

	// 現在の回転取得
	Quaternion GetRotation() const;
	// 回転設定
	void SetRotation(const Quaternion& _rotation);
	// 現在の回転へ加算
	void Rotate(const Quaternion& _rotation);

	// --- 操作要求 ---

	// ワールド空間の移動入力取得
	const Vector3& GetMoveInput() const;
	// ワールド空間の移動入力設定
	void SetMoveInput(const Vector3& _moveInput);
	// 移動入力クリア
	void ClearMoveInput();

	// ジャンプ要求
	void RequestJump();

	// 標準移動制御が有効か
	bool IsMotorEnabled() const;
	// 標準移動制御の有効状態設定
	void SetMotorEnabled(bool _enabled);

	// --- Collider設定 ---

	// カプセル中央部分の高さ取得
	float GetColliderHeight() const;
	// カプセル中央部分の高さ設定
	void SetColliderHeight(float _height);

	// カプセル半径取得
	float GetColliderRadius() const;
	// カプセル半径設定
	void SetColliderRadius(float _radius);

	// --- 移動設定 ---

	// 最大移動速度取得
	float GetMaxSpeed() const;
	// 最大移動速度設定
	void SetMaxSpeed(float _maxSpeed);

	// 地上での最大加速度取得
	float GetGroundAcceleration() const;
	// 地上での最大加速度設定
	void SetGroundAcceleration(float _acceleration);

	// 地上での最大減速度取得
	float GetGroundDeceleration() const;
	// 地上での最大減速度設定
	void SetGroundDeceleration(float _deceleration);

	// 空中での最大加速度取得
	float GetAirAcceleration() const;
	// 空中での最大加速度設定
	void SetAirAcceleration(float _acceleration);

	// ジャンプ速度取得
	float GetJumpSpeed() const;
	// ジャンプ速度設定
	void SetJumpSpeed(float _jumpSpeed);

	// 急斜面での滑り加速度取得
	float GetSlopeAcceleration() const;
	// 急斜面での滑り加速度設定
	void SetSlopeAcceleration(float _acceleration);

	// --- 接地設定 ---

	// 地面探索距離取得
	float GetGroundProbeDistance() const;
	// 地面探索距離設定
	void SetGroundProbeDistance(float _distance);

	// 歩行可能判定の最小法線内積取得
	float GetMinGroundDot() const;
	// 歩行可能判定の最小法線内積設定
	void SetMinGroundDot(float _minGroundDot);

	// --- 接地状態 ---

	// 接地状態取得
	CharacterGroundState GetGroundState() const;

	// 何らかの地面へ接触しているか
	bool IsGrounded() const;
	// 歩行可能な地面へ接触しているか
	bool IsOnWalkableGround() const;
	// 急斜面へ接触しているか
	bool IsOnSteepSlope() const;

	// 検出した地面法線取得
	const Vector3& GetGroundNormal() const;
	// 検出した地面位置取得
	const Vector3& GetGroundPoint() const;
	// カプセル底面から地面までの距離取得
	float GetGroundDistance() const;
	// 検出した地面Collider取得
	ColliderComponent GetGroundCollider() const;

	// ID
	CharacterControllerID GetID() const { return id; }
private:
	CharacterControllerID id;
};
