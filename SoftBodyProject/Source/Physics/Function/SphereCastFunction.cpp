#include <cmath>
#include <algorithm>

#include "RayCastFunction.h"
#include "SphereCastFunction.h"

// SphereRayと球の当たり判定
bool SphereCastFunction::Sphere(const SphereRay& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Vector3& _scale, float _radius)
{
	Ray ray{ _ray.origin, _ray.direction, _ray.maxDistance };

	float scaledRadius{ _radius * std::max({std::abs(_scale.x), std::abs(_scale.y), std::abs(_scale.z)}) };

	float totalRadius{ scaledRadius + _ray.radius };

	if (!RayCastFunction::Sphere(ray, _hitInfo, _center, totalRadius))
	{
		return false;
	}

	// 戻ってきたpointは接触時のキャスト球の中心。
	// 元のカプセルとの接触点へ戻す。
	_hitInfo.point -= _hitInfo.normal * _ray.radius;
	return true;
}
// SphereRayとボックスの当たり判定
bool SphereCastFunction::Box(const SphereRay& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Quaternion& _rot, const Vector3& _scale, const Vector3& _halfScale)
{
	/*
		6つの面	  : 半径分だけ外へ移動した長方形
		12本の辺  : 半径rの円柱の外側部分
		8つの頂点 : 半径rの球の外側部分
		を判定する必要がある。
		それぞれ、当たる可能性のあるtを判定し、その中で最小の値を選ぶ。
	*/
	Quaternion inverseRot{ _rot.Conjugate() };
	// o : ボックスから見たローカル始点
	Vector3 o{ inverseRot.Rotate(_ray.origin - _center) };
	// d : ボックスから見たローカル方向
	Vector3 d{ inverseRot.Rotate(_ray.direction) };
	// p : o + td

	// スケールの絶対値
	Vector3 absScale{ std::abs(_scale.x),std::abs(_scale.y),std::abs(_scale.z) };
	// h : スケール込みのボックスの各方向のサイズ
	Vector3 h{ SIMDVectorMath::Mul(absScale,_halfScale) };

	// 始点がかぶっている場合は除外をする。
	Vector3 startClosest{
	std::clamp(o.x, -h.x, h.x),
	std::clamp(o.y, -h.y, h.y),
	std::clamp(o.z, -h.z, h.z)
	};

	if ((o - startClosest).LengthSqr() < _ray.radius * _ray.radius)
	{
		return false;
	}

	/*
		面の計算
			各面について
			t = (球の半径分、面の座標をずらした座標 - 球レイの開始座標) / 進行方向の成分
			このtで求めたp = o + tdについて、
			他軸内かの判定を行う。
	*/
	float bestT{ FLT_MAX };
	bool found{ false };

	for (int axis{ 0 }; axis < 3; axis++)
	{
		if (std::abs(d[axis]) < MathConstants::EPSILON)
		{
			continue;
		}

		// 面の範囲を確認する、残り2軸
		int j{ (axis + 1) % 3 };
		int k{ (axis + 2) % 3 };

		for (int sign : { -1, 1 })
		{
			// 球の半径分、面の座標をずらした座標
			float face{ sign * (h[axis] + _ray.radius) };
			float t{ (face - o[axis]) / d[axis] };

			// tの範囲確認と、最短Tかのチェック
			if (t < 0.0f || t > _ray.maxDistance || t >= bestT)
			{
				continue;
			}

			Vector3 p{ o + d * t };

			// 他軸の範囲チェック
			if (std::abs(p[j]) > h[j] || std::abs(p[k]) > h[k])
			{
				continue;
			}

			bestT = t;
			found = true;
		}
	}
	/*
		辺・頂点の計算
			それぞれの辺を軸としたカプセルとして判定する。
	*/
	for (int axis{ 0 }; axis < 3; axis++)
	{
		// 残り2軸
		int j{ (axis + 1) % 3 };
		int k{ (axis + 2) % 3 };

		for (int sign1 : { -1, 1 })
		{
			for (int sign2 : { -1, 1 })
			{
				// Boxのローカル座標
				Vector3 edgeCenter;
				edgeCenter[j] = sign1 * h[j];
				edgeCenter[k] = sign2 * h[k];
				// 軸方向
				Vector3 axisDirection{};
				axisDirection[axis] = 1.0f;
				// 軸方向回転
				Quaternion edgeRot{ Quaternion::FromToRotation(Vector3::UP, axisDirection) };

				// この辺を軸にするカプセル
				float edgeRadius{ _ray.radius };
				float edgeHeight{ 2.0f * h[axis] };

				Ray ray
				{
					o,
					d,
					_ray.maxDistance
				};
				
				RayCastHitInfo hitInfo;
				if (RayCastFunction::Capsule(
					ray, hitInfo,
					edgeCenter, edgeRot,
					edgeRadius, edgeHeight))
				{
					// tの範囲確認と、最短Tかのチェック
					if (hitInfo.distance >= bestT)
					{
						continue;
					}

					bestT = hitInfo.distance;
					found = true;
				}
			}
		}
	}

	if (!found)
	{
		return false;
	}

	Vector3 p{ o + d * bestT };

	Vector3 closest{
		std::clamp(p.x, -h.x, h.x),
		std::clamp(p.y, -h.y, h.y),
		std::clamp(p.z, -h.z, h.z)
	};

	Vector3 localNormal{ (p - closest).Normalized() };

	// ローカルからワールドに戻す
	_hitInfo.point = _center + _rot.Rotate(closest);
	_hitInfo.normal = _rot.Rotate(localNormal);
	_hitInfo.distance = bestT;

	return true;
}
// SphereRayとカプセルの当たり判定
bool SphereCastFunction::Capsule(const SphereRay& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Quaternion& _rot, float _radius, float _height)
{
	Ray ray{ _ray.origin, _ray.direction, _ray.maxDistance };

	if (!RayCastFunction::Capsule(
		ray, _hitInfo, _center, _rot,
		_radius + _ray.radius, _height))
	{
		return false;
	}

	// 戻ってきたpointは接触時のキャスト球の中心。
	// 元のカプセルとの接触点へ戻す。
	_hitInfo.point -= _hitInfo.normal * _ray.radius;
	return true;
}
// SphereRayとAABBの当たり判定
bool SphereCastFunction::AABB(const SphereRay& _ray, const Vector3& _min, const Vector3& _max, float _currentMaxDistance, float& _tEnter)
{
	/*
		Slab法で行われている、RayCastFunctionのAABB判定を流用する。
		半径分、AABBを膨らませて判定を取ることで流用が可能。
	*/
	Vector3 expansion{ Vector3::ONE * _ray.radius };

	Ray centerRay{
		_ray.origin,
		_ray.direction,
		_ray.maxDistance
	};

	return RayCastFunction::AABB(
		centerRay,
		_min - expansion,
		_max + expansion,
		std::min(_ray.maxDistance, _currentMaxDistance),
		_tEnter
	);
}