#pragma once

#include "MDMath.h"

#include "PhysicsWorld.h"

class PhysicsAPI
{
public:
	// --- RigidBody ---

	// 作成
	static BodyID CreateRigidBody(EntityID _entity);

	// 力加算
	static void AddForce(BodyID _id, const Vector3& _force);
	// トルク加算
	static void AddTorque(BodyID _id, const Vector3& _torque);

	// 速度取得
	static const Vector3& GetVelocity(BodyID _id);
	// 速度変更
	static void SetVelocity(BodyID _id, const Vector3& _velocity);

	// 角速度取得
	static const Vector3& GetAngularVelocity(BodyID _id);
	// 角速度変更
	static void SetAngularVelocity(BodyID _id, const Vector3& _omega);

	// 質量取得
	static float GetMass(BodyID _id);
	// 質量変更
	static void SetMass(BodyID _id, float _mass);

	// 慣性テンソル取得
	static const Matrix4x4& GetInertiaTensor(BodyID _id);
	// 慣性テンソル変更
	static void SetInertiaTensor(BodyID _id, Matrix4x4& _matrix);

	// isGravity取得
	static bool GetIsGravity(BodyID _id);
	// isGravity変更
	static void SetIsGravity(BodyID _id, bool _isGravity);
	// 重力加速度取得
	static const Vector3& GetGravity(BodyID _id);
	// 重力加速度変更
	static void SetGravity(BodyID _id, const Vector3& _gravity);

	// --- コライダー系 ---
	// --- コライダー生成 ---

	// 球作成
	static ColliderID CreateSphere(EntityID _entity, float _radius);
	// Box作成
	static ColliderID CreateBox(EntityID _entity, const Vector3& _scale);

	// --- 球コライダー ---

	// 半径取得
	static float GetRadius(ColliderID _id);
	// 半径変更
	static void SetRadius(ColliderID _id, float _radius);

	// --- 箱コライダー ---

	// X方向の長さ取得
	static float GetWidth(ColliderID _id);
	// X方向の長さ変更
	static void SetWidth(ColliderID _id, float _width);
	// Y方向の長さ取得
	static float GetHeight(ColliderID _id);
	// Y方向の長さ変更
	static void SetHeight(ColliderID _id, float _height);
	// Z方向の長さ取得
	static float GetDepth(ColliderID _id);
	// Z方向の長さ変更
	static void SetDepth(ColliderID _id, float _depth);

	// --- 拘束系 ---

	// 点拘束作成
	static ConstraintID CreatePointConstraint(EntityID& _entity, const Vector3& _localOffset);
	// 点拘束にEndPoint追加
	static void AddEndPoint(ConstraintID& _id, EntityID& _entity, Vector3& _localOffset);
	// 点拘束からEndPoint除外
	static void RemoveEndPoint(ConstraintID& _id, EntityID& _entity);

	static void SetWorld(PhysicsWorld* _physicsWorld)
	{
		physicsWorld = _physicsWorld;
		colliderStorage = _physicsWorld->GetColliderStorage();
		rigidBodyStorage = _physicsWorld->GetRigidBodyStorage();
		transformStorage = _physicsWorld->GetPhysicsTransformStorage();
		constraintStorage = _physicsWorld->GetConstraintStorage();
	}
private:
	static PhysicsWorld* physicsWorld;
	static ColliderStorage* colliderStorage;
	static RigidBodyStorage* rigidBodyStorage;
	static PhysicsTransformStorage* transformStorage;
	static ConstraintStorage* constraintStorage;
};
