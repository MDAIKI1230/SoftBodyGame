#include <cmath>
#include <algorithm>

#include "RayCastFunction.h"

namespace
{
	// 球の二次方程式解く用関数
	bool SolveSphereRoots(
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
	bool CheckSphereBest(
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
}

// Rayと球の当たり判定
bool RayCastFunction::Sphere(const Ray& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Vector3& _scale, float _radius)
{
	// Rayの始点とコライダーの位置の差
	Vector3 originDist{ _ray.origin - _center };
	// 半径の2乗
	float radius{ _radius * std::max(std::max(std::abs(_scale.x), std::abs(_scale.y)), std::abs(_scale.z)) };
	float radiusSqr{ radius * radius };
	// Rayの始点とコライダーの位置の距離の2乗
	float originDistanceSqr{ originDist.LengthSqr()};

	// 始点を内包する形状は検索対象外
	if (originDistanceSqr < radiusSqr)
	{
		return false;
	}

	// レイの方向とRayの始点とコライダーの位置の差の内積
	float projectionRayDir{ Vector3::Dot(originDist, _ray.direction) };
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
	float t{ -projectionRayDir - std::sqrt(discriminant) };

	if (t < 0.0f || t > _ray.maxDistance)
	{
		return false;
	}

	// 情報を渡す
	_hitInfo.distance = t;
	_hitInfo.point = _ray.origin + _ray.direction * t;
	_hitInfo.normal = (_hitInfo.point - _center).Normalized();

	return true;
}

// Rayとボックスの当たり判定
bool RayCastFunction::Box(const Ray& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Quaternion& _rot, const Vector3& _scale, const Vector3& _halfScale)
{
	// レイの始点と方向をボックスからのローカル座標に変換
	Quaternion inverseRotation{ _rot.Conjugate() };

	Vector3 localOrigin{ inverseRotation.Rotate(_ray.origin - _center) };

	Vector3 localDir{ inverseRotation.Rotate(_ray.direction) };

	Vector3 absScale{ std::abs(_scale.x),std::abs(_scale.y),std::abs(_scale.z) };

	Vector3 halfSize{ SIMDVectorMath::Mul(_halfScale ,  absScale) };

	// 内側判定
	if (std::abs(localOrigin.x) < halfSize.x &&
		std::abs(localOrigin.y) < halfSize.y &&
		std::abs(localOrigin.z) < halfSize.z)
	{
		return false;
	}

	float tEnter{ -FLT_MAX }, tExit{ FLT_MAX };
	Vector3 localNormal;

	// 各軸方向で、Rayがボックス内にあるか判定をする
	for (int axis{ 0 }; axis < 3; axis++)
	{
		// この軸の面とRayが平行
		if (std::abs(localDir[axis]) < MathConstants::EPSILON)
		{
			// 軸の範囲外なら絶対にBoxへ入れない
			if (localOrigin[axis] < -halfSize[axis] || localOrigin[axis] > halfSize[axis])
			{
				return false;
			}

			continue;
		}

		float t1{ (-halfSize[axis] - localOrigin[axis]) / localDir[axis] };
		float t2{ (halfSize[axis] - localOrigin[axis]) / localDir[axis] };

		if (t1 > t2)
		{
			std::swap(t1, t2);
		}

		// この軸が最も遅く入る面なら、Hit面として採用
		if (t1 > tEnter)
		{
			tEnter = t1;

			localNormal = Vector3::ZERO;
			localNormal[axis] = localDir[axis] > 0.0f ? -1.0f : 1.0f;
		}

		tExit = std::min(tExit, t2);

		if (tEnter > tExit)
		{
			return false;
		}
	}

	// 入ってるか判定
	if (tEnter < 0.0f || tEnter > _ray.maxDistance)
	{
		return false;
	}
	
	_hitInfo.distance = tEnter;
	_hitInfo.point = _ray.origin + _ray.direction * tEnter;
	_hitInfo.normal = _rot.Rotate(localNormal).Normalized();

	return true;
}

// Rayとカプセルの当たり判定1
bool  RayCastFunction::Capsule(const Ray& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Quaternion& _rot, float _radius, float _height)
{
	// レイの始点と方向をボックスからのローカル座標に変換
	Quaternion inverseRotation{ _rot.Conjugate() };

	Vector3 localOrigin{ inverseRotation.Rotate(_ray.origin - _center) };

	Vector3 localDir{ inverseRotation.Rotate(_ray.direction) };

	// カプセル軸

	float heightHalf{ _height / 2.0f };

	Vector3 axisStart{ Vector3::UP * heightHalf};
	Vector3 axisEnd{ -Vector3::UP * heightHalf};

	// 内側判定
	// レイの始点のカプセルの軸上の最短距離(カプセルのローカル座標)
	Vector3 closestPoint{ MDMath::ClosestPointOnSegment(localOrigin, axisStart, axisEnd) };
	// 求めた最近点とレイの始点の差ベクトル
	Vector3 diff{ localOrigin - closestPoint };
	if (diff.LengthSqr() < _radius * _radius)
	{
		return false;
	}

	/*
		y軸を一旦考えないことで無限に長い円柱として考える。
		       よって表面の点を
		                      x^2 + y^2 = r^2
		と考えるとx,yにそれぞれRayの式を成分ごとに代入して
		(O_x + tD_x)^2 + (O_z + tD_z)^2 = r^2

		これを二次方程式として解く
	*/

	// それぞれ二次方程式の解の公式に対応

	float a{ localDir.x * localDir.x + localDir.z * localDir.z };

	float b{ localOrigin.x * localDir.x + localOrigin.z * localDir.z };

	float c{ localOrigin.x * localOrigin.x + localOrigin.z * localOrigin.z - _radius * _radius };

	// 判別式
	float discriminant{ b * b - a * c };

	float bestT{ FLT_MAX };
	Vector3 bestNormal{};
	bool found{ false };

	// 解がある場合のみ計算
	if (discriminant >= 0.0f && std::abs(a) >= MathConstants::EPSILON)
	{
		float sqrtDiscriminant{ std::sqrt(discriminant) };

		// 解候補
		float candidates[2]
		{
			(-b - sqrtDiscriminant) / a,
			(-b + sqrtDiscriminant) / a
		};

		// Y軸を判定して当たってるかちゃんと判定する
		for (float t : candidates)
		{
			if (t < 0.0f || t > _ray.maxDistance)
			{
				continue;
			}

			float hitY{ localOrigin.y + localDir.y * t };

			// 中央の円柱部分から上下にはみ出している
			if (hitY < -heightHalf || hitY > heightHalf)
			{
				continue;
			}

			Vector3 localHitPoint{ localOrigin + localDir * t };

			Vector3 localNormal{ localHitPoint.x,0.0f,localHitPoint.z };

			localNormal = localNormal.Normalized();

			if (t < bestT)
			{
				bestT = t;
				bestNormal = localNormal;
				found = true;
			}
		}
	}

	
	// カプセルの軸の始点の球判定
	if (CheckSphereBest(localOrigin, localDir, _ray.maxDistance, axisStart, _radius, heightHalf, Vector3::UP, bestT, bestNormal))
	{
		found = true;
	}

	// カプセルの軸の終点の球判定
	if (CheckSphereBest(localOrigin, localDir, _ray.maxDistance, axisEnd, _radius, heightHalf, -Vector3::UP, bestT, bestNormal))
	{
		found = true;
	}

	// ここまで見つかってないなら当たらない。
	if (found == false)
	{
		return false;
	}

	_hitInfo.distance = bestT;
	_hitInfo.point = _ray.origin + _ray.direction * bestT;
	_hitInfo.normal = _rot.Rotate(bestNormal).Normalized();

	return true;
}

bool RayCastFunction::AABB(const Ray& _ray, const Vector3& _min, const Vector3& _max, float _currentMaxDistance, float& _tEnter)
{
	// ボックスの回転ない版
	float tEnter{ 0.0f };
	float tExit{ _currentMaxDistance };

	for (int axis{ 0 }; axis < 3; ++axis)
	{
		if (std::abs(_ray.direction[axis])
			< MathConstants::EPSILON)
		{
			if (_ray.origin[axis] < _min[axis] ||
				_ray.origin[axis] > _max[axis])
			{
				return false;
			}

			continue;
		}

		float t1{ (_min[axis] - _ray.origin[axis]) / _ray.direction[axis] };

		float t2{ (_max[axis] - _ray.origin[axis]) / _ray.direction[axis] };

		if (t1 > t2)
		{
			std::swap(t1, t2);
		}

		tEnter = std::max(tEnter, t1);
		tExit = std::min(tExit, t2);

		if (tEnter > tExit)
		{
			return false;
		}
	}

	_tEnter = tEnter;
	return true;
}
