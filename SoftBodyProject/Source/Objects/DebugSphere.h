#pragma once

#include "ObjectBase.h"

class DebugSphere:public ObjectBase
{
public:
	// --- 更新系 ---

	void Update() override;
	void FixedUpdate() override;

	// --- 衝突系 ---

	void OnCollisionEnter() override;
	void OnCollision() override;
	void OnCollisionExit() override;
};