#include <cmath>
#include <algorithm>

#include "RayCastFunction.h"
#include "SphereCastFunction.h"

// SphereRayと球の当たり判定
bool SphereCastFunction::Sphere(const SphereRay& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Vector3& _scale, float _radius)
{
	float targetRadius{ _radius * std::max({
	std::abs(_scale.x),
	std::abs(_scale.y),
	std::abs(_scale.z)
		}) };

	float totalRadius{ targetRadius + _ray.radius };
	/*
		P(t) = O + tD : 線分の公式
		O : 始点
		t : 移動距離(0 ≦ t ≦ maxDistance)
		D : 方向(単位ベクトル)
		
		| P(t) - C | = R + t : 満たしたい二次方程式
		C : 円の中心
		R : 円の半径
		r : スフィアレイの半径

		m = O - Cとすると
		| m + tD |^2 = (R + r)^2

		m^2 + 2mtD + (tD)^2 = (R + r)^2
		b = m・D, c = m・m - (R + r)^2, D・D = 1 とすると
		t^2 + 2bt + c = 0

		t = -b - √(b^2 - c)
	*/

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

		o : ボックスから見たローカル始点
		d : ボックスから見たローカル方向
		p : o + td
		h : スケール込みのボックスの各方向のサイズ

		// 面の計算
			各面について
			t = (球の半径分面の座標をずらした座標 - 球の中心の開始座標) / 進行方向の成分
			このtで求めたp = o + tdについて、
			他軸内かの判定を行う。

		// 辺の計算
			カプセルと同様に、pと線とレイの半径の二次方程式を作り
			出た点についてスケール内にあるかどうかを調べる
		// 頂点の計算
			通常の点と球の判定と同じように行う。
	*/
	return true;
}
// SphereRayとカプセルの当たり判定
bool SphereCastFunction::Capsule(const SphereRay& _ray, RayCastHitInfo& _hitInfo, const Vector3& _center, const Quaternion& _rot, float _radius, float _height)
{
	/*
		o : カプセルから見たローカル始点
		d : カプセルから見たローカル方向

		// 中央の円柱部分に対して
		(o_x + td_x)^2+(o_z + td_z)^2 = (R + r)^2

		a = d_x^2 + d_z^2

		b = o_xd_x + o_zd_z

		c = o_x^2 + o_z^2 -(R + r)^2

		とすると、

		t = (-b ± √(b^2 - ac)) / a

		これを満たすtに対して
		- halfHeight ≦ o_y + td_y ≦ halfHeight
		halfHeight : カプセルの半分の高さ
		を満たすかを調べる。

		// 上下の半円部分に対して、
		Top    : 0.0f,  halfHeight, 0.0f
		Bottom : 0.0f, -halfHeight, 0.0f

		この二つを球の中心として、球との当たり判定を行う。
	*/
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