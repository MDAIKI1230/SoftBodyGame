#include "RigidBodyComponentStorage.h"

RigidBodyComponent* RigidBodyComponentStorage::Add(int _entity, RigidBodyComponent&& _obj)
{
	// --- 値を確保 ---

	// --- 速度系 ---

	// 力
	force.emplace_back();
	// 速度
	velocity.emplace_back();
	// 推定移動位置
	expectedPos.emplace_back();

	// --- 角速度系 ---

	// トルク
	torque.emplace_back();
	// 角速度
	angularVelocity.emplace_back();
	// 推定回転角度
	expectedRot.emplace_back();

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
	inverseInertiaTensor.emplace_back();

	// マテリアルID(一旦なし)
	physicsMatrialID.emplace_back(-1);

	return sparseSet.Add(_entity, std::move(_obj));
}
