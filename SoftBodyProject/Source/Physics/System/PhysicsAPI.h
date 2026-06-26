#pragma once

#include "MDMath.h"

#include "WorldStorage.h"

class PhysicsAPI
{
public:
	// --- RigidBody ---
	
	// 力加算
	static void AddForce(int _handle, const Vector3& _force);
	// トルク加算
	static void AddTorque(int _handle, const Vector3& _torque);

	// 速度取得
	static const Vector3& GetVelocity(int _handle);
	// 速度変更
	static void SetVelocity(int _handle, const Vector3& _velocity);

	// 角速度取得
	static const Vector3& GetAngularVelocity(int _handle);
	// 角速度変更
	static void SetAngularVelocity(int _handle, const Vector3& _omega);

	// 質量取得
	static float GetMass(int _handle);
	// 質量変更
	static void SetMass(int _handle, float _mass);

	// 慣性テンソル取得
	static const Matrix4x4& GetInertiaTensor(int _handle);
	// 慣性テンソル変更
	static void SetInertiaTensor(int _handle, Matrix4x4& _matrix);

	// isGravity取得
	static bool GetIsGravity(int _handle);
	// isGravity変更
	static void SetIsGravity(int _handle, bool _isGravity);
	// 重力加速度取得
	static const Vector3& GetGravity(int _handle);
	// 重力加速度変更
	static void SetGravity(int _handle, const Vector3& _gravity);

	// --- Collider ---
	// --- BOX

	// X方向の長さ取得
	static float GetWidth(int _handle);
	// X方向の長さ変更
	static void SetWidth(int _handle, float _width);
	// Y方向の長さ取得
	static float GetHeight(int _handle);
	// Y方向の長さ変更
	static void SetHeight(int _handle, float _heibht);
	// Z方向の長さ取得
	static float GetDepth(int _handle);
	// Z方向の長さ変更
	static void SetDepth(int _handle, float _depth);

	static void SetWorld(WorldStorage* _world) { world = _world; }
private:
	static WorldStorage* world;
};
