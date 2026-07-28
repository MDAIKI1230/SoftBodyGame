#pragma once

#include "ObjectBase.h"
#include "WorldStorage.h"

class DebugSphere :public ObjectBase
{
public:
	// コンストラクタ
	DebugSphere(WorldStorage* world, EntityID _entity);
	// --- 更新系 ---

	void Update() override;
	void FixedUpdate() override;

	// --- 衝突系 ---

	void OnCollisionEnter() override;
	void OnCollision() override;
	void OnCollisionExit() override;
};
