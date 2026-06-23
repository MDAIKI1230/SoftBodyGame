#pragma once

struct RigidBodyComponent
{
public:
	// --- 力加算系 ---

	// 力追加
	void AddForce(Vector3 _force);
	// トルク追加
	void AddTrque(Vector3 _trque);

	// 

	// ハンドル関係
	int GetHnadle() { return handle; }
	void SetHandle(int _handle) { handle = _handle; }
private:
	int handle;
};
