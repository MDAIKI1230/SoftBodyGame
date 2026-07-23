#include "RigidBodyStorage.h"


void RigidBodyStorage::CreateRigidBody(EntityID _entity, PhysicsTransformID _transformID, BodyID _id)
{
	// --- 速度系 ---

	// 力
	force.emplace_back();
	// 速度
	velocity.emplace_back();

	// --- 角速度系 ---

	// トルク
	torque.emplace_back();
	// 角速度
	angularVelocity.emplace_back();

	// --- 重力系 ---

	// 重力フラグ
	isGravity.emplace_back(true);
	// 重力加速度
	gravity.emplace_back(0.0, -980.0f, 0.0f);

	// --- 質量系 ---

	// 質量(0除算を避けるため)
	mass.emplace_back(1.0f);
	// 質量の逆数
	inverseMass.emplace_back(1.0f);
	// 慣性テンソルの逆数
	localInverseInertiaTensor.emplace_back();
	// ワールド慣性テンソルの逆数
	worldInverseInertiaTensor.emplace_back();

	// --- 衝突用 ---

	// 推定移動位置
	pastPos.emplace_back();
	// 推定姿勢
	pastRot.emplace_back();

	// --- Dirty系 ---

	// ローカル慣性テンソル変更
	localInertiaDirty.emplace_back(true);

	// マテリアルID(一旦なし)
	physicsMatrialID.emplace_back(-1);

	// ID
	id.emplace_back(_id);
}

BodyID RigidBodyStorage::Remove(uint32_t _index)
{
	// 力
	force[_index] = std::move(force.back());
	force.pop_back();
	// 速度
	velocity[_index] = std::move(velocity.back());
	velocity.pop_back();

	// --- 角速度系 ---

	// トルク
	torque[_index] = std::move(torque.back());
	torque.pop_back();
	// 角速度
	angularVelocity[_index] = std::move(angularVelocity.back());
	angularVelocity.pop_back();

	// --- 重力系 ---

	// 重力フラグ
	isGravity[_index] = std::move(isGravity.back());
	isGravity.pop_back();
	// 重力加速度
	gravity[_index] = std::move(gravity.back());
	gravity.pop_back();

	// --- 質量系 ---

	// 質量(0除算を避けるため)
	// 推定回転角度
	mass[_index] = std::move(mass.back());
	mass.pop_back();
	// 質量の逆数
	inverseMass[_index] = std::move(inverseMass.back());
	inverseMass.pop_back();
	// ローカル慣性テンソルの逆数
	localInverseInertiaTensor[_index] = std::move(localInverseInertiaTensor.back());
	localInverseInertiaTensor.pop_back();
	// ワールド慣性テンソルの逆数
	worldInverseInertiaTensor[_index] = std::move(worldInverseInertiaTensor.back());
	worldInverseInertiaTensor.pop_back();

	// ローカル慣性テンソル変更
	localInertiaDirty[_index] = std::move(localInertiaDirty.back());
	localInertiaDirty.pop_back();

	// --- 衝突用 ---

	// 推定移動位置
	pastPos[_index] = std::move(pastPos.back());
	pastPos.pop_back();
	// 推定姿勢
	pastRot[_index] = std::move(pastRot.back());
	pastRot.pop_back();

	// マテリアルID(一旦なし)
	physicsMatrialID[_index] = std::move(physicsMatrialID.back());
	physicsMatrialID.pop_back();

	// swap-removeしたときの移動したID
	BodyID movedId{ id[_index] };

	// 移動後のIDの修正
	slots[movedId.GetIndex()].denseIndex = slots[_index].denseIndex;

	return movedId;
}
