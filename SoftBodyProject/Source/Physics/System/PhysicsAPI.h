#pragma once

#include "MDMath.h"

#include "IWorld.h"

class PhysicsAPI
{
public:
	// 力加算
	static void AddForce(int _handle, const Vector3& _force);
	// トルク加算
	static void AddTorque(int _handle, const Vector3& _torque);

	// 速度取得
	static const Vector3& GetVelocity(int _handle) const;
	// 速度変更
	static void SetVelocity(int _handle, const Vector3& _velocity);

	// 角速度取得
	static const Vector3& GetAngularVelocity(int _handle) const;
	// 角速度変更
	static void SetAngularVelocity(int _handle, const Vector3& _omega);

	// 質量取得
	static float GetMass(int _handle) const;
	// 質量変更
	static void SetMass(int _handle, float _mass);

	// 慣性テンソル取得
	static const Matrix4x4& GetInertiaTensor(int _handle) const;
	// 慣性テンソル変更
	static void SetInertiaTensor(int _handle, Matrix4x4& _matrix);

	// isGravity取得
	static bool GetIsGravity(int _handle) const;
	// isGravity変更
	static void SetIsGravity(int _handle, bool _isGravity);
	// 重力加速度取得
	static const Vector3& GetGravity(int _handle) const;
	// 重力加速度変更
	static void SetGravity(int _handle, const Vector3& _gravity);

	static void SetWorld(IWorld* _world) { world = _world; }
private:
	static IWorld* world;
};
