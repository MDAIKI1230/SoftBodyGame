#pragma once

#include <vector>
#include <unordered_map>

#include "TransformComponentStorage.h"
#include "ColliderComponentStorage.h"
#include "ColliderProjection.h"
#include "CollPair.h"

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
	/// <summary>
	/// 軸射影重なり判定
	/// </summary>
	/// <param name="projectionAxisValues">判定する軸のコンテナ</param>
	/// <param name="actives">結果を入れる</param>
	void CheckProjectionAxisValueCross(const std::vector<ColliderProjection>& projectionAxisValues);
private:
	// ブロードフェーズ用コンテナ
	// X軸射影
	std::vector<ColliderProjection> colliderProjectionXValues;
	// y軸射影
	std::vector<ColliderProjection> colliderProjectionYValues;
	// z軸射影
	std::vector<ColliderProjection> colliderProjectionZValues;

	// AABBの衝突判定用のカウンター
	std::unordered_map<CollPair, int> crossCountMap;

	// ナローフェーズをするペア
	std::vector<CollPair> narrowPairs;
};