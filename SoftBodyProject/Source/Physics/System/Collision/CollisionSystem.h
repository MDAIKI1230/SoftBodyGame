#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "TransformComponentStorage.h"
#include "SphereColliderComponentStorage.h"
#include "ProjectionStorage.h"
#include "CollisionManifoldBuffer.h"

#include "ColliderProjection.h"
#include "CollPair.h"
#include "Simplex.h"

#include "FixedUpdateSystem.h"

class CollisionSystem
{
public:
	// 更新
	void FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager, CollisionManifoldBuffer* _manifoldBuffer);
private:
	// ブロードフェーズ
	void BroadPhase(TransformComponentStorage* _transformStorage, SphereColliderComponentStorage* _sphereStorage);
	// ナローフェーズ
	void NarrowPhase(TransformComponentStorage* _transformStorage, SphereColliderComponentStorage* _sphereStorage, EventManager* _eventManager);
	// 終了処理
	void End();
	/// <summary>
	/// 軸射影重なり判定
	/// </summary>
	/// <param name="projectionAxisValues">判定する軸のコンテナ</param>
	/// <param name="actives">結果を入れる</param>
	void CheckProjectionAxisValueCross(const std::vector<ColliderProjection>& _projectionAxisValues);
	/// <summary>
	/// GJKアルゴリズムによる当り判定
	/// </summary>
	/// <typeparam name="A">Aの形状タグ</typeparam>
	/// <typeparam name="B">Bの形状タグ</typeparam>
	/// <typeparam name="AS">Aの形状のストレージクラス</typeparam>
	/// <typeparam name="BS">Bの形状のストレージクラス</typeparam>
	/// <param name="_storageA">Aの形状のストレージ</param>
	/// <param name="_handleA">Aのエンティティハンドル</param>
	/// <param name="_storageB">Bの形状のストレージ</param>
	/// <param name="_handleB">Bのエンティティハンドル</param>
	/// <param name="_transformStorage">トランスフォームストレージ</param>
	/// <returns>当たったか</returns>
	template<class A,class B,class AS,class BS>
	bool GJK(
		AS* _storageA,int _handleA,
		BS* _storageB,int _handleB,
		TransformComponentStorage* _transformStorage);

	// --- 各形状ごとの引数の当り判定 ---

	bool Solve(SphereColliderComponentStorage* _strageA, SphereColliderComponentStorage* _strageB, SphereSpherePair& pair, TransformComponentStorage* _transformStorage);

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
	// --- ブロードフェーズ用コンテナ ---
	// X軸射影
	std::vector<ColliderProjection> colliderProjectionXValues;
	// y軸射影
	std::vector<ColliderProjection> colliderProjectionYValues;
	// z軸射影
	std::vector<ColliderProjection> colliderProjectionZValues;

	// AABBの衝突判定用のカウンター
	std::unordered_map<SphereSpherePair, int> crossCountMap{};

	// ナローフェーズをするペア
	std::vector<SphereSpherePair> narrowPairs;

	// --- 衝突管理 ---
	// 今回当たってたやつ
	std::unordered_set<SphereSpherePair> currentFramePair;
	// 前回当たってたやつ
	std::unordered_set<SphereSpherePair> prevFramePair;

	// 一旦置く
	ProjectionStorage projectionStorage;
};
