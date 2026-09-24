#pragma once

#include "MDMath.h"

#include "CameraConstants.h"

#include "CollisionFilter.h"

/*
* カメラをFPSやTPSの視点操作を実装するためのコンポーネント
*/
struct CameraRigComponent
{
public:
	// コンストラクタ
	CameraRigComponent() = default;

	// カメラモード取得
	CameraMode GetMode() const noexcept { return mode; }
	// カメラモード変更
	void SetMode(CameraMode _mode) noexcept { mode = _mode; }
	// 追従対象取得
	EntityID GetFollowTarget() const noexcept { return followTarget; }
	// 今フレームの視点回転量変更
	void SetLookDelta(const Vector2& _delta) noexcept { lookDelta = _delta; }
	// 今フレームの視点回転量加算
	void AddLookDelta(const Vector2& _delta) noexcept { lookDelta += _delta; }
	// 追従対象変更
	void SetFollowTarget(EntityID _target) noexcept { followTarget = _target; }
	// Yaw取得
	float GetYaw() const noexcept { return yaw; }
	// Yaw変更
	void SetYaw(float _yaw) noexcept { yaw = _yaw; }
	// Pitch取得
	float GetPitch() const noexcept { return pitch; }
	// Pitch変更
	void SetPitch(float _pitch) noexcept { pitch = _pitch; }
	// 最小Pitch取得
	float GetMinPitch() const noexcept { return minPitch; }
	// 最小Pitch変更
	void SetMinPitch(float _pitch) noexcept { minPitch = _pitch; }
	// 最大Pitch取得
	float GetMaxPitch() const noexcept { return maxPitch; }
	// 最大Pitch変更
	void SetMaxPitch(float _pitch) noexcept { maxPitch = _pitch; }

	// --- FPS ---

	// FPS位置オフセット取得
	const Vector3& GetFirstPersonOffset() const noexcept { return firstPersonOffset; }
	// FPS位置オフセット変更
	void SetFirstPersonOffset(const Vector3& _offset) noexcept { firstPersonOffset = _offset; }

	// --- TPS ---

	// Pivotオフセット取得
	const Vector3& GetPivotOffset() const noexcept { return pivotOffset; }
	// Pivotオフセット変更
	void SetPivotOffset(const Vector3& _offset) noexcept { pivotOffset = _offset; }
	// TPS肩越しオフセット取得
	const Vector3& GetShoulderOffset() const noexcept { return shoulderOffset; }
	// TPS肩越しオフセット変更
	void SetShoulderOffset(const Vector3& _offset) noexcept { shoulderOffset = _offset; }
	// 今フレームの視点回転量取得
	const Vector2& GetLookDelta() const noexcept { return lookDelta; }
	// 今フレームの視点回転量消費
	void ClearLookDelta() noexcept { lookDelta = Vector2{}; }
	// TPS距離取得
	float GetDistance() const noexcept { return distance; }
	// TPS距離変更
	void SetDistance(float _distance) noexcept { distance = _distance; }
	// 位置追従の平滑化時間取得
	float GetPositionDamping() const noexcept { return positionDamping; }
	// 位置追従の平滑化時間変更
	void SetPositionDamping(float _damping) noexcept { positionDamping = _damping; }

	// レイのフィルター取得
	const CollisionFilter& GetRayFilter()const { return rayFilter; }
	// レイのフィルター変更
	void GetRayFilter(const CollisionFilter& _rayFilter) { rayFilter = _rayFilter; }
private:
	// FPS/TPS
	CameraMode mode{ CameraMode::TPS };
	// 目標
	EntityID followTarget;
	// 一フレームで加える変化量
	Vector2 lookDelta;
	// 縦軸角度(弧度法)
	float yaw{ 0.0f };
	// 横軸角度(弧度法)
	float pitch{ 0.0f };
	// 最小ピッチ(弧度法)
	float minPitch{ -MathConstants::PI_FLT / 3.0f};
	// 最大ピッチ(弧度法)
	float maxPitch{ MathConstants::PI_FLT / 3.0f };

	// --- FPSで使う ---

	// FPSの場合のオフセット
	Vector3 firstPersonOffset;

	// --- TPSで使う ---

	// 位置のオフセット
	Vector3 pivotOffset;
	// カメラの左右オフセット
	Vector3 shoulderOffset;
	// 追従の滑らかさ(0なら即時追従)
	float positionDamping{ 0.0f };
	// TPS距離
	float distance{ 300.0f };

	// レイのフィルター
	CollisionFilter rayFilter;
};