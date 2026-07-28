#pragma once

#include "MDMath.h"
#include "StorageAccessorsMacros.h"

#include "BodyID.h"
#include "PhysicsTransformID.h"

class RigidBodyStorage
{
	// --- 速度系 ---
	// 力
	MD_STORAGE_READ_WRITE_COLUMN(Vector3, Force, forces);
	// 速度
	MD_STORAGE_READ_WRITE_COLUMN(Vector3, Velocity, velocities);
	// --- 角速度系 ---
	// トルク
	MD_STORAGE_READ_WRITE_COLUMN(Vector3, Torque, torques);
	// 角速度
	MD_STORAGE_READ_WRITE_COLUMN(Vector3, AngularVelocity, angularVelocities);
	// --- 重力系 ---
	// 重力フラグ
	MD_STORAGE_READ_WRITE_COLUMN(bool, IsGravity, isGravities);
	// 重力加速度
	MD_STORAGE_READ_WRITE_COLUMN(Vector3, Gravity, gravities);
	// --- 質量系 ---
	// 質量
	MD_STORAGE_WRITE_ORIGINAL_COLUMN(float, Mass, masses);
public:
	// 質量の代入
	void SetMass(uint32_t _index, float _mass);
	// 質量の逆数
	MD_STORAGE_READ_ONLY_COLUMN(float, InverseMass, inverseMasses);
	// ローカル慣性テンソルの逆数
	MD_STORAGE_READ_WRITE_COLUMN(Matrix4x4, LocalInverseInertiaTensor, localInverseInertiaTensors);
	// --- 衝突用 ---
	// 推定移動位置
	MD_STORAGE_READ_WRITE_COLUMN(Vector3, PastPosition, pastPositions);
	// 推定姿勢
	MD_STORAGE_READ_WRITE_COLUMN(Quaternion, PastRotation, pastRotations);
	// --- Diary系 ---
	// ローカル慣性テンソル変更フラグ
	MD_STORAGE_WRITE_ORIGINAL_COLUMN(bool, LocalInertiaDiary, localInertiaDiaries);
public:
	// 計算が完了したときに呼ぶ関数
	void LocalInertiaCalcSucces(uint32_t _index);

	// ID
	MD_STORAGE_READ_ONLY_COLUMN(BodyID, ID, ids);

public:
	// コンストラクタ
	RigidBodyStorage() = default;

	// RigidBody作成
	void Create(BodyID _id);

	// 破棄
	BodyID Remove(uint32_t _index);

private:
	std::unordered_map<PhysicsTransformID, BodyID> transformMap;
};
