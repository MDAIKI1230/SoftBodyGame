#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "PhysicsTransformStorage.h"
#include "ColliderStorage.h"
#include "CollisionManifoldBuffer.h"
#include "NarrowPhasePairBuilder.h"

#include "ColliderProjection.h"
#include "ColliderProjectionData.h"
#include "CollisionPair.h"

#include "EventManager.h"

class CollisionSystem
{
public:
	// 更新
	void FixedUpdate(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer, EventManager* _eventManager);
private:
	// ブロードフェーズ
	void BroadPhase(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage);
	// ペアの対応付け処理
	void Dispatch(ColliderStorage* _colliderStorage);
	// ナローフェーズ
	void NarrowPhase(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer, EventManager* _eventManager);
	// 終了処理
	void End();
	/// <summary>
	/// 軸射影重なり判定
	/// </summary>
	/// <param name="projectionAxisValues">判定する軸のコンテナ</param>
	/// <param name="actives">結果を入れる</param>
	void CheckProjectionAxisValueCross(std::vector<ColliderProjection>& _projectionAxisValues);

	// --- 各形状ごとの引数の当り判定 ---
	template<class A, class B, class PairList>
	void Solve(
		const std::vector<PairList>& pairList,
		PhysicsTransformStorage* _transformStorage,
		ColliderStorage* _colliderStorage,
		CollisionManifoldBuffer* _manifoldBuffer,
		EventManager* _eventManager);
	template<>
	void Solve<ColliderTag::BoxTag, ColliderTag::BoxTag, CollisionPair::BoxBoxPair>(
		const std::vector<CollisionPair::BoxBoxPair>& pairList,
		PhysicsTransformStorage* _transformStorage,
		ColliderStorage* _colliderStorage,
		CollisionManifoldBuffer* _manifoldBuffer,
		EventManager* _eventManager);
	template<>
	void Solve<ColliderTag::SphereTag, ColliderTag::SphereTag, CollisionPair::SphereSpherePair>(
		const std::vector<CollisionPair::SphereSpherePair>& pairList,
		PhysicsTransformStorage* _transformStorage,
		ColliderStorage* _colliderStorage,
		CollisionManifoldBuffer* _manifoldBuffer,
		EventManager* _eventManager);
	template<>
	void Solve<ColliderTag::SphereTag, ColliderTag::BoxTag, CollisionPair::SphereBoxPair>(
		const std::vector<CollisionPair::SphereBoxPair>& pairList,
		PhysicsTransformStorage* _transformStorage,
		ColliderStorage* _colliderStorage,
		CollisionManifoldBuffer* _manifoldBuffer,
		EventManager* _eventManager);
	template<>
	void Solve<ColliderTag::BoxTag, ColliderTag::SphereTag, CollisionPair::BoxSpherePair>(
		const std::vector<CollisionPair::BoxSpherePair>& pairList,
		PhysicsTransformStorage* _transformStorage,
		ColliderStorage* _colliderStorage,
		CollisionManifoldBuffer* _manifoldBuffer,
		EventManager* _eventManager);

	void RegisterEvent(
		ColliderID _colliderA,
		ColliderID _colliderB,
		ColliderStorage* _colliderStorage,
		EventManager* _eventManager);

	void RegisterExitEvent(ColliderStorage* _colliderStorage, EventManager* _eventManager);

	// --- ソート ---
	void InsertionSort(std::vector<ColliderProjection>& _projectionValues);
	/// <summary>
	/// スワップ
	/// </summary>
	/// <param name="_projectionValues">射影コンテナ</param>
	/// <param name="a">スワップするインデックス一つ目</param>
	/// <param name="b">スワップするインデックス二つ目</param>
	void Swap(std::vector<ColliderProjection>& _projectionValues, int _a, int _b);
private:
	NarrowPhasePairBuilder narrowPhasePairBuilder;
	// Pairの追加用Dipatchテーブル
	using AddPairFunc = void (NarrowPhasePairBuilder::*)(ColliderID, ColliderID);

	AddPairFunc AddPairTable[(int)ColliderType::COUNT][(int)ColliderType::COUNT]
	{
		// 　　　             球　　　               |　             　 箱
		{
			&NarrowPhasePairBuilder::AddSphereSphere , &NarrowPhasePairBuilder::AddSphereBox // 球
		},
		{
			&NarrowPhasePairBuilder::AddBoxSphere,     &NarrowPhasePairBuilder::AddBoxBox    // 箱
		}
	};

	// --- ブロードフェーズ用コンテナ ---
	// X軸射影
	std::vector<ColliderProjection> colliderProjectionXValues;
	// y軸射影
	std::vector<ColliderProjection> colliderProjectionYValues;
	// z軸射影
	std::vector<ColliderProjection> colliderProjectionZValues;

	// AABBの衝突判定用のカウンター
	std::unordered_map<CollisionPair::Pair, int> crossCountMap{};

	// ナローフェーズへ移行できるペア(Dispatch処理でペアの形状を判定する)
	std::vector<CollisionPair::Pair> broadClearPairs;

	// --- 衝突管理 ---
	// 今回当たってたやつ
	std::unordered_set<CollisionPair::Pair> currentFramePair;
	// 前回当たってたやつ
	std::unordered_set<CollisionPair::Pair> prevFramePair;

	// コライダーIDのindexに対応させる。
	std::vector<ColliderProjectionData> projectionDatas;
};
