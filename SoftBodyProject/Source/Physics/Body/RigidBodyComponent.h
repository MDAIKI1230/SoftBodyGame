#pragma once

struct RigidBodyComponent
{
public:
	int GetHnadle() { return handle; }
	void SetHandle(int _handle) { handle = _handle; }
private:
	int handle;
};
