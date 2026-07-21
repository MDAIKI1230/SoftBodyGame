#pragma once

#include "ColliderID.h"
#include "Face.h"
#include "Edge.h"
#include "MinkowskiVertex.h"
#include "Simplex.h"

#include "ColliderStorage.h"
#include "PhysicsTransformStorage.h"
#include "CollisionManifoldBuffer.h"

namespace GJKEPA
{
	/// <summary>
	/// GJKアルゴリズムによる当り判定
	/// </summary>
	/// <typeparam name="A">Aの形状タグ</typeparam>
	/// <typeparam name="B">Bの形状タグ</typeparam>
	/// <param name="_colliderA">AのコライダーID</param>
	/// <param name="_colliderB">BのコライダーID</param>
	/// <param name="_colliderStorage">コライダーストレージ</param>
	/// <param name="_transformStorage">トランスフォームストレージ</param>
	/// <param name="_manifoldBuffer">結果を入れる</param>
	/// <returns>当たったか</returns>
	template<class A, class B>
	bool GJK(
		ColliderID _colliderA, ColliderID _colliderB,
		ColliderStorage* _colliderStorage,
		PhysicsTransformStorage* _transformStorage,
		CollisionManifoldBuffer* _manifoldBuffer);

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

	template<class A, class B>
	void EPA(
		ColliderID _colliderA, ColliderID _colliderB,
		PhysicsTransformStorage* _transformStorage,
		ColliderStorage* _colliderStorage,
		CollisionManifoldBuffer* _manifoldBuffer, Simplex& _simplex);

	void ComputeFace(Face& _face, std::vector<MinkowskiVertex>& _vertices);
	void AddEdge(Edge& _edge, std::vector<Edge>& _edges);
	ContactPoint CalcContactPosition(Face& _face, std::vector<MinkowskiVertex>& _vertices);
}
