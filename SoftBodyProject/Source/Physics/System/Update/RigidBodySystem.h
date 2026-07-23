#pragma once

#include "BodyStorage.h"
#include "ColliderStorage.h"
#include "PhysicsTransformStorage.h"

class RigidBodySystem
{
public:
	// コンストラクタ
	RigidBodySystem() = default;
	// 更新
	void FixedUpdate(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage);
private:
	// 重力適応
	void UpdateGravity(BodyStorage* _bodyStorage);
	// 位置更新
	void UpdatePosition(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage);
	// 回転更新
	void UpdateRotation(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage);
	// 逆慣性テンソル更新
	void UpdateInverseInertiaTensor(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage);
	// 終了処理
	void End(BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage);

	// --- 慣性テンソル計算 ---
	Matrix4x4 GenerateBoxInverseInertiaTensor(uint32_t _transformIndex, ColliderID _colliderID, float _mass, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage);
	Matrix4x4 GenerateSphereInverseInertiaTensor(uint32_t _transformIndex, ColliderID _colliderID, float _mass, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage);
};
