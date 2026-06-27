#pragma once

#include <vector>

#include "MDMath.h"

#include "BodySlot.h"
#include "BodyID.h"

class RigidBodyStorage
{
public:
	// コンストラクタ
	RigidBodyStorage() = default;

	// RigidBody作成
	BodyID CreateRigidBody(EntityID _entity);

	// 破棄
	void Destroy(BodyID _id);

	// 生存確認
	bool IsAlive(BodyID _id) const;
	// 実データのインデックス
	size_t GetDenseIndex(BodyID _id) const;
	// 持ってるEntity
	EntityID GetOwnerEntity(BodyID _id) const;
public:
	std::vector<BodySlot> slots;
	std::vector<size_t> freeSlots;

	// --- 速度系 ---

	// 力
	std::vector<Vector3> force;
	// 速度
	std::vector<Vector3> velocity;
	// 推定移動位置
	std::vector<Vector3> position;

	// --- 角速度系 ---

	// トルク
	std::vector<Vector3> torque;
	// 角速度
	std::vector<Vector3>angularVelocity;
	// 推定回転角度
	std::vector<Quaternion> rotation;

	// --- 重力系 ---

	// 重力フラグ
	std::vector<bool> isGravity;
	// 重力加速度
	std::vector<Vector3> gravity;
	// サイズも質量に関連付けしとく
	std::vector<Vector3> scale;

	// --- 質量系 ---

	// 質量
	std::vector<float> mass;
	// 質量の逆数
	std::vector<float> inverseMass;
	// 慣性テンソル
	std::vector<Matrix4x4> inertiaTensor;
	// 慣性テンソルの逆数
	std::vector<Matrix4x4> inverseInertiaTensor;

	// マテリアルID
	std::vector<int> physicsMatrialID;

	// ID
	std::vector<BodyID> id;
private:
	BodyID GenerateBodyID(size_t _denseIndex, EntityID _ownerEntity);
};
