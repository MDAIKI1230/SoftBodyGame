#pragma once

#include "RigidBodyStorage.h"
#include "ColliderStorage.h"
#include "PhysicsTransformStorage.h"

class RigidBodySystem
{
public:
	// コンストラクタ
	RigidBodySystem() = default;
	// 更新
	void FixedUpdate(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, ColliderStorage* _colliderStorage);
private:
	// 重力適応
	void UpdateGravity(RigidBodyStorage* _bodyStorage);
	// 位置更新
	void UpdatePosition(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage);
	// 回転更新
	void UpdateRotation(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage);
	// 逆慣性テンソル更新
	void UpdateInverseInertiaTensor(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, ColliderStorage* _colliderStorage);
	// 終了処理
	void End(RigidBodyStorage* _bodyStorage, ColliderStorage* _colliderStorage);

	// --- 慣性テンソル計算 ---
	Matrix4x4 GenerateBoxInverseInertiaTensor(const uint32_t& _transformIndex, const ColliderID& _colliderID, float _mass, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage);
	Matrix4x4 GenerateSphereInverseInertiaTensor(const uint32_t& _transformIndex, const ColliderID& _colliderID, float _mass, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage);
};
