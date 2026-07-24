#include "RigidBodyStorage.h"


// 質量の代入
void RigidBodyStorage::SetMass(uint32_t _index, float _mass)
{
	// 0以下はダメ。
	if (_mass <= 0)
	{
		return;
	}
	masses[_index] = _mass;
	// 質量の逆数を計算
	inverseMasses[_index] = 1 / _mass;
	// 慣性テンソルの変化フラグも立てておく
	localInertiaDiaries[_index] = true;
}

// 計算が完了したときに呼ぶ関数
void RigidBodyStorage::LocalInertiaCalcSucces(uint32_t _index)
{
	localInertiaDiaries[_index] = false;
}

void RigidBodyStorage::Create(BodyID _id)
{
	// --- 速度系 ---

	// 力
	forces.emplace_back();
	// 速度
	velocities.emplace_back();

	// --- 角速度系 ---

	// トルク
	torques.emplace_back();
	// 角速度
	angularVelocities.emplace_back();

	// --- 重力系 ---

	// 重力フラグ
	isGravities.emplace_back(true);
	// 重力加速度
	gravities.emplace_back(0.0, -980.0f, 0.0f);

	// --- 質量系 ---

	// 質量(0除算を避けるため)
	masses.emplace_back(1.0f);
	// 質量の逆数
	inverseMasses.emplace_back(1.0f);
	// 慣性テンソルの逆数
	localInverseInertiaTensors.emplace_back();

	// --- 衝突用 ---

	// 推定移動位置
	pastPositions.emplace_back();
	// 推定姿勢
	pastRotations.emplace_back();

	// --- Diary系 ---

	// ローカル慣性テンソル変更
	localInertiaDiaries.emplace_back(true);

	// ID
	ids.emplace_back(_id);
}

BodyID RigidBodyStorage::Remove(uint32_t _index)
{
	// 力
	forces[_index] = std::move(forces.back());
	forces.pop_back();
	// 速度
	velocities[_index] = std::move(velocities.back());
	velocities.pop_back();

	// --- 角速度系 ---

	// トルク
	torques[_index] = std::move(torques.back());
	torques.pop_back();
	// 角速度
	angularVelocities[_index] = std::move(angularVelocities.back());
	angularVelocities.pop_back();

	// --- 重力系 ---

	// 重力フラグ
	isGravities[_index] = std::move(isGravities.back());
	isGravities.pop_back();
	// 重力加速度
	gravities[_index] = std::move(gravities.back());
	gravities.pop_back();

	// --- 質量系 ---

	// 質量(0除算を避けるため)
	// 推定回転角度
	masses[_index] = std::move(masses.back());
	masses.pop_back();
	// 質量の逆数
	inverseMasses[_index] = std::move(inverseMasses.back());
	inverseMasses.pop_back();
	// ローカル慣性テンソルの逆数
	localInverseInertiaTensors[_index] = std::move(localInverseInertiaTensors.back());
	localInverseInertiaTensors.pop_back();

	// ローカル慣性テンソル変更
	localInertiaDiaries[_index] = std::move(localInertiaDiaries.back());
	localInertiaDiaries.pop_back();

	// --- 衝突用 ---

	// 推定移動位置
	pastPositions[_index] = std::move(pastPositions.back());
	pastPositions.pop_back();
	// 推定姿勢
	pastRotations[_index] = std::move(pastRotations.back());
	pastRotations.pop_back();

	// swap-removeしたときの移動したID
	return ids[_index];
}
