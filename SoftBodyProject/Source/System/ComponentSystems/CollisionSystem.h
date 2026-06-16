#pragma once

#include <vector>

#include "TransformComponentStorage.h"
#include "ColliderComponentStorage.h"

#include "FixedUpdateSystem.h"

class CollisionSystem :public FixedUpdateSystem
{
public:
	// 更新
	void FixedUpdate(IWorld* world) override;
private:
	// ブロードフェーズ
	void BroadPhase(TransformComponentStorage* transformStorage, std::vector<int>* colliderEntities);
	// ナローフェーズ
	void NarrowPhase(TransformComponentStorage* transformStorage, std::vector<int>* colliderStorage);
private:
	// ナローフェーズをするペア
	std::vector<int[2]> narrowPairs;
};