#pragma once

struct RigidBodyComponent
{
public:
	// --- 力加算系 ---

	// 力加算
	void AddForce(const Vector3& _force);
	// トルク加算
	void AddTorque(const Vector3& _torque);

	// --- 速度計 ---

	// 速度取得
	const Vector3& GetVelocity() const;
	// 速度変更
	void SetVelocity(const Vector3& velocity);

	// --- 各速度系 ---

	// 角速度取得
	const Vector3& GetAngularVelocity() const;
	// 角速度変更
	void SetAngularVelocity(const Vector3& omega);

	// --- 質量系 ---

	// 質量取得
	float GetMass() const;
	// 質量変更
	void SetMass(float _mass);

	// --- 慣性テンソル系 ---

	// 慣性テンソル取得
	const Matrix4x4& GetInertiaTensor() const;
	// 慣性テンソル変更
	void SetInertiaTensor(Matrix4x4& _matrix);

	// --- 重力系 ---

	// isGravity取得
	bool GetIsGravity() const;
	// isGravity変更
	void SetIsGravity(bool _isGravity);
	// 重力加速度取得
	const Vector3& GetGravity() const;
	// 重力加速度変更
	void SetGravity(const Vector3& _gravity);

	// --- ハンドル関係 ---

	int GetHnadle() { return handle; }
	void SetHandle(int _handle) { handle = _handle; }
private:
	int handle;
};
