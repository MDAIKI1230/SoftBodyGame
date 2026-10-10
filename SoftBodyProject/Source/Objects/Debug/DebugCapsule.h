#pragma once

#include "MonoBehaviour.h"
#include "WorldStorage.h"

class DebugCapsule :public MonoBehaviour
{
public:
	// コンストラクタ
	DebugCapsule(WorldStorage* world, EntityID _entity);
	// --- 更新系 ---

	void Update() override;
	void FixedUpdate() override;

	// --- 衝突系 ---

	void OnCollisionEnter(CollisionInfo _info) override;
	void OnCollision(CollisionInfo _info) override;
	void OnCollisionExit(CollisionInfo _info) override;
};
