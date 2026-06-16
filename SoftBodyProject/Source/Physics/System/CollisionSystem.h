#pragma once

#include <vector>

#include "TransformComponentStorage.h"
#include "ColliderComponentStorage.h"
#include "ColliderProjection.h"

#include "FixedUpdateSystem.h"

class CollisionSystem :public FixedUpdateSystem
{
public:
	// 更新
	void FixedUpdate(IWorld* world) override;
private:
	// ブロードフェーズ
	void BroadPhase(TransformComponentStorage* transformStorage, ColliderComponentStorage* colliderStorage);
	// ナローフェーズ
	void NarrowPhase(TransformComponentStorage* transformStorage, ColliderComponentStorage* colliderStorage);
private:
	// ブロードフェーズ用コンテナ
	// X軸射影
	std::vector<ColliderProjection> colliderProjectionXValues;

	// ナローフェーズをするペア
	std::vector<int[2]> narrowPairs;
};