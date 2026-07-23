#pragma once

#include <vector>
#include <unordered_map>

#include "MDMath.h"

#include "BodySlot.h"
#include "BodyID.h"
#include "PhysicsTransformID.h"

class RigidBodyStorage
{
public:
	// コンストラクタ
	RigidBodyStorage() = default;

	// RigidBody作成
	void CreateRigidBody(EntityID _entity, PhysicsTransformID _transformID, BodyID _id);

	// 破棄
	BodyID Remove(uint32_t _index);
public:
	std::vector<BodySlot> slots;
	std::vector<uint32_t> freeSlots;

	// --- 速度系 ---

	// 力
	std::vector<Vector3> force;
	// 速度
	std::vector<Vector3> velocity;

	// --- 角速度系 ---

	// トルク
	std::vector<Vector3> torque;
	// 角速度
	std::vector<Vector3>angularVelocity;

	// --- 重力系 ---

	// 重力フラグ
	std::vector<bool> isGravity;
	// 重力加速度
	std::vector<Vector3> gravity;

	// --- 質量系 ---

	// 質量
	std::vector<float> mass;
	// 質量の逆数
	std::vector<float> inverseMass;
	// 慣性テンソル
	std::vector<Matrix4x4> inertiaTensor;
	// ローカル慣性テンソルの逆数
	std::vector<Matrix4x4> localInverseInertiaTensor;
	// ワールド慣性テンソルの逆数
	std::vector<Matrix4x4> worldInverseInertiaTensor;

	// --- 衝突用 ---

	// 推定移動位置
	std::vector<Vector3> pastPos;
	// 推定姿勢
	std::vector<Quaternion> pastRot;

	// --- Dirty系 ---
	
	// ローカル慣性テンソル変更
	std::vector<bool> localInertiaDirty;

	// マテリアルID
	std::vector<int> physicsMatrialID;

	// ID
	std::vector<BodyID> id;
private:
	BodyID GenerateBodyID(size_t _denseIndex, EntityID _ownerEntity, PhysicsTransformID _transformID);
private:
	std::unordered_map<PhysicsTransformID, BodyID> transformMap;
};
