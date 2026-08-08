#pragma once

#include "Matrix/Matrix.h"
#include "Quaternion/Quaternion.h"
#include "Vector/Vector2/Vector2.h"
#include "Vector/Vector2/Vector2Int.h"
#include "Vector/Vector3/Vector3.h"
#include "Vector/Vector3/Vector3Int.h"
#include "Vector/Vector4/Vector4.h"
#include "Transform/Transform.h"
#include "Matrix/MatGenerateFunc.h"
#include "Constants/MathConstants.h"
#include "Vector/SIMD/SIMDVectorMath.h"

namespace MDMath
{
	/// <summary>
	/// 線分上の点の最近点
	/// </summary>
	/// <param name="_point">点</param>
	/// <param name="_segment">線分の始点</param>
	/// <param name="_start">線分の終点</param>
	/// <returns></returns>
	Vector3 ClosestPointOnSegment(const Vector3& _point, const Vector3& _start, const Vector3& _end);

	/// <summary>
	/// 線分と点の距離
	/// </summary>
	/// <param name="_point">点</param>
	/// <param name="_start">線分の始点</param>
	/// <param name="_end">線分の終点</param>
	/// <returns></returns>
	float DistancePointToSegment(const Vector3& _point, const Vector3& _start, const Vector3& _end);

	// 線分と線分の最近点関数の結果
	struct ClosestPointsBetweenSegmentsResult
	{
		Vector3 pointA;
		Vector3 pointB;
	};

	/// <summary>
	/// 線分と線分の最近点
	/// </summary>
	/// <param name="_startA">線分Aの始点</param>
	/// <param name="_endA">線分Aの終点</param>
	/// <param name="_startB">線分Bの始点</param>
	/// <param name="_endB">線分Bの終点</param>
	/// <returns></returns>
	ClosestPointsBetweenSegmentsResult ClosestSegmentOnSegment(const Vector3& _startA, const Vector3& _endA, const Vector3& _startB, const Vector3& _endB);

	// 線分とAABBの最近点関数の結果
	struct ClosestPointsBetweenSegmentAndAABBResult
	{
		Vector3 pointSegment;
		Vector3 pointAABB;

		// 線分のパラメータ値
		float parameterOnSegment{ 0.0f };

		// 2つの最近点間の距離の2乗
		float distanceSqr{ 0.0f };

		// 線分がAABBに接触、または侵入している
		bool intersects{ false };
	};

	ClosestPointsBetweenSegmentAndAABBResult ClosestPointsBetweenSegmentAndAABB(const Vector3& _segmentStart, const Vector3& _segmentEnd, const Vector3& _boxCenter, const Vector3& _boxSize);

	// 線分とOBBの最近点関数の結果
	struct ClosestPointsBetweenSegmentAndOBBResult
	{
		Vector3 pointSegment;
		Vector3 pointOBB;

		// 線分のパラメータ値
		float parameterOnSegment{ 0.0f };

		// 2つの最近点間の距離の2乗
		float distanceSqr{ 0.0f };

		// 線分がAABBに接触、または侵入している
		bool intersects{ false };
	};

	ClosestPointsBetweenSegmentAndOBBResult ClosestPointsBetweenSegmentAndOBB(const Vector3& _segmentStart, const Vector3& _segmentEnd, const Vector3& _boxCenter, const Quaternion _boxRot, const Vector3& _boxSize);
};
