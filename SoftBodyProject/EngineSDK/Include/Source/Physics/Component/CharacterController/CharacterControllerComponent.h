#pragma once

#include "MDMath.h"

#include "EntityID.h"
#include "CharacterControllerID.h"

#include "CharacterControllerConstants.h"

#include "ColliderComponent.h"
#include "RigidBodyComponent.h"

struct CharacterControllerComponent
{
public:
	// コンストラクタ
	CharacterControllerComponent(EntityID _entity, const RigidBodyComponent& _rigidBody);

	// --- 移動 ---

	// 現在の位置取得
	Vector3 GetPosition() const;
	// 位置設定
	void SetPosition(const Vector3& _position);
	// 速度取得
	const Vector3& GetVelocity() const;
	// 速度設定
	void SetVelocity(const Vector3& _velocity);

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

	// --- 接地探索Filter ---

	// 接地探索用コリジョンフィルター取得
	CollisionFilter GetCollisionFilter();
	// 接地探索用コリジョンフィルター設定
	void SetCollisionFilter(const CollisionFilter& _filter);

	// --- 移動設定 ---

	// 最大移動速度取得
	float GetMaxSpeed() const;
	// 最大移動速度設定
	void SetMaxSpeed(float _maxSpeed);

	// 最大駆動力の計算に使う斜面角度取得(度)
	float GetMaxMoverForceSlopeAngleDeg() const;
	// 最大駆動力の計算に使う斜面角度設定(度)
	void SetMaxMoverForceSlopeAngleDeg(float _angleDeg);

	// 地上での最大加速度取得
	float GetGroundAcceleration() const;
	// 地上での最大加速度設定
	void SetGroundAcceleration(float _acceleration);

	// 地上での最大減速度取得
	float GetGroundDeceleration() const;
	// 地上での最大減速度設定
	void SetGroundDeceleration(float _deceleration);

	// 最大落下速度取得
	float GetMaxFallSpeed() const;
	// 最大落下速度設定
	void SetMaxFallSpeed(float _maxFallSpeed);

	// 空中での最大加速度取得
	float GetAirAcceleration() const;
	// 空中での最大加速度設定
	void SetAirAcceleration(float _acceleration);

	// 空中での最大減速度取得
	float GetAirDeceleration() const;
	// 空中での最大減速度設定
	void SetAirDeceleration(float _deceleration);

	// ジャンプ速度取得
	float GetJumpSpeed() const;
	// ジャンプ速度設定
	void SetJumpSpeed(float _jumpSpeed);

	// ジャンプ状態を保持する最大時間取得(秒)
	float GetMaxJumpTime() const;
	// ジャンプ状態を保持する最大時間設定(秒)
	void SetMaxJumpTime(float _maxJumpTime);

	// 現在のジャンプ経過時間取得(秒)
	float GetJumpTime() const;

	// 空中の駆動力倍率取得
	float GetAirForceScale() const;
	// 空中の駆動力倍率設定
	void SetAirForceScale(float _scale);

	// 空中の最高速倍率取得
	float GetAirSpeedScale() const;
	// 空中の最高速倍率設定
	void SetAirSpeedScale(float _scale);

	// 急斜面の駆動力倍率取得
	float GetSteepSlopeForceScale() const;
	// 急斜面の駆動力倍率設定
	void SetSteepSlopeForceScale(float _scale);

	// 急斜面の最高速倍率取得
	float GetSteepSlopeSpeedScale() const;
	// 急斜面の最高速倍率設定
	void SetSteepSlopeSpeedScale(float _scale);

	// --- 接地設定 ---

	// 地面探索距離・接地拘束の支持距離取得
	float GetGroundProbeDistance() const;
	// 地面探索距離・接地拘束の支持距離設定
	void SetGroundProbeDistance(float _distance);

	// 歩行可能判定の最小法線内積取得
	float GetMinGroundDot() const;
	// 歩行可能判定の最小法線内積設定
	void SetMinGroundDot(float _minGroundDot);

	// スカートの半径取得
	float GetSkirtRadius() const;
	// スカートの半径設定
	void SetSkirtRadius(float _radius);

	// スカートのレイ本数取得
	int GetSkirtRayCount() const;
	// スカートのレイ本数設定
	void SetSkirtRayCount(int _count);

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