#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "TransformComponentStorage.h"
#include "ColliderComponentStorage.h"
#include "ObjectManager.h"

#include "ColliderProjection.h"
#include "CollPair.h"
#include "Simplex.h"

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
	void NarrowPhase(TransformComponentStorage* transformStorage, ColliderComponentStorage* colliderStorage, ObjectManager* objectManager);
	// 終了処理
	void End();
	/// <summary>
	/// 軸射影重なり判定
	/// </summary>
	/// <param name="projectionAxisValues">判定する軸のコンテナ</param>
	/// <param name="actives">結果を入れる</param>
	void CheckProjectionAxisValueCross(const std::vector<ColliderProjection>& projectionAxisValues);
	/// <summary>
	/// GJKアルゴリズム
	/// </summary>
	/// <param name="collider01">コライダー01</param>
	/// <param name="collider02">コライダー02</param>
	/// <returns>衝突判定(当たったらture)</returns>
	bool GJK(
		const ColliderComponent& collider01, const TransformComponent& transform01,
		const ColliderComponent& collider02, const TransformComponent& transform02);

	// --- SimplexSolve ---
	/// <summary>
	/// 今のSimlpexから適切な方向を計算する
	/// </summary>
	/// <param name="_output">アウトプットするところ</param>
	/// <returns>衝突したかどうか</returns>
	bool SimplexSolve(Simplex& _simplex, Vector3& _output);
	// 一点の時の計算
	bool SolvePoint(Simplex& _simplex, Vector3& _output);
	// 二点の時(線分)の計算
	bool SolveLine(Simplex& _simplex, Vector3& _output);
	// 三点の時(三角形)の計算
	bool SolveTriangle(Simplex& _simplex, Vector3& _output);
	// 四点の時(四面体)の計算
	bool SolveTetrahedron(Simplex& _simplex, Vector3& _output);
private:
	// --- ブロードフェーズ用コンテナ ---
	// X軸射影
	std::vector<ColliderProjection> colliderProjectionXValues;
	// y軸射影
	std::vector<ColliderProjection> colliderProjectionYValues;
	// z軸射影
	std::vector<ColliderProjection> colliderProjectionZValues;

	// AABBの衝突判定用のカウンター
	std::unordered_map<CollPair, int> crossCountMap{};

	// ナローフェーズをするペア
	std::vector<CollPair> narrowPairs;

	// --- 衝突管理 ---
	// 今回当たってたやつ
	std::unordered_set<CollPair> currentFramePair;
	// 前回当たってたやつ
	std::unordered_set<CollPair> prevFramePair;
};