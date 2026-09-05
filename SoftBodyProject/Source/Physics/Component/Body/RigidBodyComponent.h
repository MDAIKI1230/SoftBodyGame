#pragma once

#include "EntityID.h"
#include "BodyID.h"

#include "BodyConstants.h"

struct RigidBodyComponent
{
public:
	// --- コンストラクタ ---
	
	// デフォルトコンストラクタ(ストレージに追加できないよ)
	RigidBodyComponent() = default;

	RigidBodyComponent(EntityID _entity);

	// --- 力加算系 ---

	// 力加算
	void AddForce(const Vector3& _force);
	// トルク加算
	void AddTorque(const Vector3& _torque);

	// --- 速度計 ---

	// 速度取得
	const Vector3& GetVelocity() const;
	// 速度変更
	void SetVelocity(const Vector3& velocity);

	// --- 各速度系 ---

	// 角速度取得
	const Vector3& GetAngularVelocity() const;
	// 角速度変更
	void SetAngularVelocity(const Vector3& omega);

	// --- 質量系 ---

	// 質量取得
	float GetMass() const;
	// 質量変更
	void SetMass(float _mass);

	// --- 重力系 ---

	// isGravity取得
	bool GetIsGravity() const;
	// isGravity変更
	void SetIsGravity(bool _isGravity);
	// 重力加速度取得
	const Vector3& GetGravity() const;
	// 重力加速度変更
	void SetGravity(const Vector3& _gravity);

	// 回転制限取得
	RigidBodyRotationLock GetRotationLock() const;
	// 回転制限変更
	void SetRotationLock(RigidBodyRotationLock _lock);
	// 移動制限取得
	RigidBodyPositionLock GetPositionLock() const;
	// 移動制限変更
	void SetPositionLock(RigidBodyPositionLock _lock);

	// ID取得
	BodyID GetID() const { return id; }
private:
	BodyID id{};
};
