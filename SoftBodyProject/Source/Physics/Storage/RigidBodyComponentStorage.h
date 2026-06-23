#pragma once

#include "MDMath.h"

#include "SparseSetStorageBase.h"

#include "RigidBodyComponent.h"

class RigidBodyComponentStorage :public SparseSetStorageBase<RigidBodyComponent>
{
public:
	/*
		すべてのコンテナが、SparseSetのIDと一致していることに気を付ける
		例：
		　Entity]IDからのコンポーネント取得関数

		　int id{ sparse[_entity] };
		  return &dense[id];

		  この時のIDは他のコンテナにも対応している

		  Velocityならこれ、
		  velocity[id];

		  よって、コンポーネント内のハンドルは、idになる。
	*/

	// --- 速度系 ---

	// 力
	std::vector<Vector3> force;
	// 速度
	std::vector<Vector3> velocity;
	// 推定移動位置
	std::vector<Vector3> expectedPos;
	
	// --- 角速度系 ---

	// トルク
	std::vector<Vector3> torque;
	// 角速度
	std::vector<float>angularVelocity;
	// 推定回転角度
	std::vector<Quaternion> expectedQuat;

	// --- 重力系 ---

	// 重力フラグ
	std::vector<bool> isGravity;
	// 重力の強さと方向
	std::vector<Vector3> garavity;

	// --- 質量系 ---

	// 質量
	std::vector<float> mass;
	// 質量の逆数
	std::vector<float> inverseMass;
	// 慣性テンソルの逆数
	std::vector<Matrix4x4> inverseInertiaTensor;

	// マテリアルID
	std::vector<int> physicsMatrialID;
};
