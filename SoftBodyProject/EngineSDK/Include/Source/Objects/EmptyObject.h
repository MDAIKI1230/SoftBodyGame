#pragma once

#include "MonoBehaviour.h"

class EmptyObject :public MonoBehaviour
{
public:
	// コンストラクタ
	EmptyObject(WorldStorage* world, EntityID _entity);
	// --- 更新系 ---

	void Update() override;
	void FixedUpdate() override;

	// --- 衝突系 ---

	void OnCollisionEnter(CollisionInfo _info) override;
	void OnCollision(CollisionInfo _info) override;
	void OnCollisionExit(CollisionInfo _info) override;
};
