#include <cmath>

#include "RayCastHelper.h"

// 球の二次方程式解く用関数
bool RayCastHelper::SolveSphereRoots(
	const Vector3& _origin, const Vector3& _direction,
	const Vector3& _center, float _radius,
	float& _tNear, float& _tFar)
{

	// Rayの始点とコライダーの位置の差
	Vector3 originDist{ _origin - _center };
	// 半径の2乗
	float radiusSqr{ _radius * _radius };
	// Rayの始点とコライダーの位置の距離の2乗
	float originDistanceSqr{ originDist.LengthSqr() };

	// レイの方向とRayの始点とコライダーの位置の差の内積
	float projectionRayDir{ Vector3::Dot(originDist, _direction) };
	// Rayの始点とコライダーの位置の距離の2乗と半径の2乗の差
	float diffSqr{ originDistanceSqr - radiusSqr };

	// 球の外側にいて、さらに球から離れる方向
	if (projectionRayDir > 0.0f && diffSqr > 0.0f)
	{
		return false;
	}

	/*
					  Ray上の点は P(t) = O + tD
					O：Rayの始点
					D：Rayの進行方向(正規化済み)
					t：始点からの距離

				   球上の点は|P - C|^2 = r^2 を満たす
					C：球の中心座標
					r：球の半径
			これに、PにRayの式を代入さらに、展開すると
					  |(O + tD) - C|^2 = r^2
		　                 m = O - Cとすると
							|m + tD|^2 = r^2 これを内積として展開
					(m + tD)・(m + tD) = r^2
	  m^2 + 2t(m・D) + t^2(D・D) - r^2 = 0 Dは正規化済みなのでD・D = 1
	  　　　m^2 + 2t(m・D) + t^2 - r^2 = 0
		 t^2 + 2bt + c = 0

	　					ここまでで、
	 　				　projectionRayDir = (m・D)
							　 diffSqr = m^2
										と考える
	*/
	// 判別式
	float discriminant{ projectionRayDir * projectionRayDir - diffSqr };

	// 解なしとして処理
	if (discriminant < 0.0f)
	{
		return false;
	}

	// 解を求める
	_tNear = -projectionRayDir - std::sqrt(discriminant);
	_tFar = -projectionRayDir + std::sqrt(discriminant);

	return true;
}

// カプセルの時の球の判定関数
bool RayCastHelper::CheckSphereBest(
	const Vector3& _origin, const Vector3& _direction, float _maxDistance,
	const Vector3& _center, float _radius, float _heightHalf, Vector3 _outward,
	float& _bestT, Vector3& _bestNormal)
{
	float tNear, tFar;
	if (SolveSphereRoots(_origin, _direction, _center, _radius, tNear, tFar))
	{
		float candidates[2]{ tNear, tFar };
		bool updated{ false };

		for (float t : candidates)
		{
			if (t < 0.0f ||
				t > _maxDistance ||
				t >= _bestT)
			{
				continue;
			}

			Vector3 point{ _origin + _direction * t };

			// 外側半球か
			if (Vector3::Dot(point - _center, _outward) < 0.0f)
			{
				continue;
			}

			_bestT = t;
			_bestNormal = (point - _center).Normalized();
			updated = true;
		}

		return updated;
	}

	return false;
}