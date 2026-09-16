#include <algorithm>

#include "ContactFunction.h"

// 球 VS 球
bool ContactFunction::SphereSphere(ColliderID _colliderA, ColliderID _colliderB, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	// Aの情報取得
	float radiusA{ _colliderStorage->GetSphereColliderRadius(_colliderA) };
	PhysicsTransformID transformA{ _colliderStorage->GetTransformID(_colliderA) };
	// Bの情報取得
	float radiusB{ _colliderStorage->GetSphereColliderRadius(_colliderB) };
	PhysicsTransformID transformB{ _colliderStorage->GetTransformID(_colliderB) };

	// スケールの適応(書く方向で最大を選ぶ)
	Vector3 scale{ _transformStorage->GetScale(transformA) };
	radiusA *= std::max(std::max(scale.x, scale.y), scale.z);

	scale = _transformStorage->GetScale(transformB);
	radiusB *= std::max(std::max(scale.x, scale.y), scale.z);

	// 半径の合計
	float totalRadius{ radiusA + radiusB };

	// 位置のオフセット計算
	Vector3 positionA{ _transformStorage->GetPosition(transformA) +
		_transformStorage->GetRotation(transformA).Rotate(_colliderStorage->GetSphereColliderOffsetPosition(_colliderA)) };
	Vector3 positionB{ _transformStorage->GetPosition(transformB) +
		_transformStorage->GetRotation(transformB).Rotate(_colliderStorage->GetSphereColliderOffsetPosition(_colliderB)) };

	// 差
	Vector3 diff{ positionB - positionA };

	float distSqr{ diff.LengthSqr() };
	// 判定
	if (distSqr <= totalRadius * totalRadius)
	{
		// 衝突情報の追加
		Manifold manifold;
		manifold.colliderA = _colliderA;
		manifold.colliderB = _colliderB;

		// 長さが0なら正規化できないので上方向にする
		if (distSqr >= MathConstants::EPSILON)
		{
			manifold.normal = diff.Normalized();
		}
		else
		{
			manifold.normal = Vector3::UP;
		}

		ContactPoint contactPoint;
		contactPoint.penetration = totalRadius - std::sqrtf(distSqr);

		Vector3 worldPointA{ positionA + manifold.normal * radiusA };
		Vector3 worldPointB{ positionB - manifold.normal * radiusB };

		contactPoint.positionLocalA =
			_transformStorage->GetRotation(transformA).Conjugate().Rotate(
				worldPointA - _transformStorage->GetPosition(transformA));

		contactPoint.positionLocalB =
			_transformStorage->GetRotation(transformB).Conjugate().Rotate(
				worldPointB - _transformStorage->GetPosition(transformB));

		manifold.AddPoints(contactPoint);

		_manifoldBuffer->Add(manifold);

		return true;
	}

	return false;
}

// 球 VS ボックス
bool ContactFunction::SphereBox(ColliderID _colliderSphere, ColliderID _colliderBox, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	// Sphereの情報取得
	PhysicsTransformID transformSphere{ _colliderStorage->GetTransformID(_colliderSphere) };
	Vector3 scale{ _transformStorage->GetScale(transformSphere) };
	float radius{ _colliderStorage->GetSphereColliderRadius(_colliderSphere) * std::max(std::max(scale.x, scale.y), scale.z) };
	// Boxの情報取得
	Vector3 halfScaleBox{ _colliderStorage->GetBoxColliderScale(_colliderBox) * 0.5f };
	PhysicsTransformID transformBox{ _colliderStorage->GetTransformID(_colliderBox) };
	Quaternion rotBox{ _transformStorage->GetRotation(transformBox) * _colliderStorage->GetBoxColliderOffsetRotation(_colliderBox) };

	halfScaleBox = SIMDVectorMath::Mul(halfScaleBox, _transformStorage->GetScale(transformBox));

	// 基底ベクトル
	Vector3 axis[]
	{
		rotBox.Rotate(Vector3::RIGHT),
		rotBox.Rotate(Vector3::UP),
		rotBox.Rotate(Vector3::FORWARD)
	};

	// 位置のオフセット計算
	Vector3 positionSphere{ _transformStorage->GetPosition(transformSphere) +
		_transformStorage->GetRotation(transformSphere).Rotate(_colliderStorage->GetSphereColliderOffsetPosition(_colliderSphere)) };
	Vector3 positionBox{ _transformStorage->GetPosition(transformBox) +
		_transformStorage->GetRotation(transformBox).Rotate(_colliderStorage->GetBoxColliderOffsetPosition(_colliderBox)) };

	// ローカル座標
	Vector3 temp{ positionSphere - positionBox };
	Vector3 localCirclePosition{ Vector3::Dot(axis[0],temp),Vector3::Dot(axis[1],temp),Vector3::Dot(axis[2],temp) };

	// 矩形内にクランプ
	Vector3 latestPoint{ 
		std::clamp(localCirclePosition.x, -halfScaleBox.x, halfScaleBox.x),
		std::clamp(localCirclePosition.y, -halfScaleBox.y, halfScaleBox.y),
		std::clamp(localCirclePosition.z, -halfScaleBox.z, halfScaleBox.z),
	};

	// 最近点と円の中心の距離
	float dist{ Vector3::DistanceSqr(localCirclePosition, latestPoint) };

	// 距離が円の半径よりも大きいなら当たっていない
	if (dist > radius * radius)
	{
		return false;
	}

	// ---衝突計算
	Manifold manifold;

	manifold.colliderA = _colliderSphere;
	manifold.colliderB = _colliderBox;

	// 中心点が矩形の外なら
	if (latestPoint != localCirclePosition)
	{
		Vector3 deff{ latestPoint - localCirclePosition };

		ContactPoint contactPoint;
		// 重なり深さ計算
		contactPoint.penetration = radius - deff.Length();
		// 法線計算
		Vector3 normalLocal = deff.Normalize();
		// ワールド座標に変換
		manifold.normal = axis[0] * normalLocal.x + axis[1] * normalLocal.y + axis[2] * normalLocal.z;

		// ワールド位置を計算しローカルに直す

		Vector3 worldPointSphere{ positionSphere + manifold.normal * radius };
		Vector3 worldPointBox{ positionBox + (axis[0] * latestPoint.x + axis[1] * latestPoint.y + axis[2] * latestPoint.z) };

		contactPoint.positionLocalA =
			_transformStorage->GetRotation(transformSphere).Conjugate().Rotate(
				worldPointSphere - _transformStorage->GetPosition(transformSphere));
		contactPoint.positionLocalB =
			_transformStorage->GetRotation(transformBox).Conjugate().Rotate(
				worldPointBox - _transformStorage->GetPosition(transformBox));

		// 点追加
		manifold.AddPoints(contactPoint);

		_manifoldBuffer->Add(manifold);

		return true;
	}

	// 中心点が矩形の中

	// 4辺との距離
	float distances[6]
	{
	localCirclePosition.x + halfScaleBox.x, // -X
    halfScaleBox.x - localCirclePosition.x, // +X

    localCirclePosition.y + halfScaleBox.y, // -Y
    halfScaleBox.y - localCirclePosition.y, // +Y

    localCirclePosition.z + halfScaleBox.z, // -Z
    halfScaleBox.z - localCirclePosition.z  // +Z
	};

	// 対応した法線
	Vector3 normals[6]
	{
		 axis[0], // -X面
		-axis[0], // +X面

		 axis[1], // -Y面
		-axis[1], // +Y面

		 axis[2], // -Z面
		-axis[2]  // +Z面
	};

	// 最小距離の探索
	int minIndex{ 0 };
	for (int i = 1; i < 6; i++)
	{
		if (distances[i] < distances[minIndex])
		{
			minIndex = i;
		}
	}

	// 最小距離の法線
	manifold.normal = normals[minIndex];
	ContactPoint contactPoint;
	// 重なり深さ計算
	contactPoint.penetration = distances[minIndex] + radius;

	// ワールド位置を計算しローカルに直す

	Vector3 boxPointLocal{ localCirclePosition };

	switch (minIndex)
	{
	case 0: boxPointLocal.x = -halfScaleBox.x; break;
	case 1: boxPointLocal.x = halfScaleBox.x; break;
	case 2: boxPointLocal.y = -halfScaleBox.y; break;
	case 3: boxPointLocal.y = halfScaleBox.y; break;
	case 4: boxPointLocal.z = -halfScaleBox.z; break;
	case 5: boxPointLocal.z = halfScaleBox.z; break;
	}

	Vector3 worldPointSphere{ positionSphere - manifold.normal * radius };
	Vector3 worldPointBox{ positionBox + (axis[0] * boxPointLocal.x + axis[1] * boxPointLocal.y + axis[2] * boxPointLocal.z) };

	contactPoint.positionLocalA = 
		_transformStorage->GetRotation(transformSphere).Conjugate().Rotate(
			worldPointSphere - _transformStorage->GetPosition(transformSphere));
	contactPoint.positionLocalB =
		_transformStorage->GetRotation(transformBox).Conjugate().Rotate(
			worldPointBox - _transformStorage->GetPosition(transformBox));

	// 点追加
	manifold.AddPoints(contactPoint);

	_manifoldBuffer->Add(manifold);

	return true;
}

// ボックス VS ボックス
bool ContactFunction::BoxBox(ColliderID _colliderA, ColliderID _colliderB, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	// Aの情報取得
	Vector3 halfScaleA{ _colliderStorage->GetBoxColliderScale(_colliderA) * 0.5f };
	PhysicsTransformID transformA{ _colliderStorage->GetTransformID(_colliderA) };
	Quaternion rotA{ _transformStorage->GetRotation(transformA) * _colliderStorage->GetBoxColliderOffsetRotation(_colliderA) };
	// Bの情報取得
	Vector3 halfScaleB{ _colliderStorage->GetBoxColliderScale(_colliderB) * 0.5f };
	PhysicsTransformID transformB{ _colliderStorage->GetTransformID(_colliderB) };
	Quaternion rotB{ _transformStorage->GetRotation(transformB) * _colliderStorage->GetBoxColliderOffsetRotation(_colliderB) };

	// スケールの適応
	halfScaleA = SIMDVectorMath::Mul(halfScaleA, _transformStorage->GetScale(transformA));

	halfScaleB = SIMDVectorMath::Mul(halfScaleB, _transformStorage->GetScale(transformB));

	// 分離軸候補
	Vector3 candidateAxisA[]
	{
		rotA.Rotate(Vector3::RIGHT),
		rotA.Rotate(Vector3::UP),
		rotA.Rotate(Vector3::FORWARD)
	};

	Vector3 candidateAxisB[]
	{
		rotB.Rotate(Vector3::RIGHT),
		rotB.Rotate(Vector3::UP),
		rotB.Rotate(Vector3::FORWARD)
	};


	// 2つの矩形の半分をまとめあげる
	float halfsA[]
	{
		halfScaleA.x,halfScaleA.y,halfScaleA.z,
	};
	float halfsB[]
	{
		halfScaleB.x,halfScaleB.y,halfScaleB.z,
	};

	// 位置のオフセット計算
	Vector3 positionA{ _transformStorage->GetPosition(transformA) +
		_transformStorage->GetRotation(transformA).Rotate(_colliderStorage->GetBoxColliderOffsetPosition(_colliderA)) };
	Vector3 positionB{ _transformStorage->GetPosition(transformB) +
		_transformStorage->GetRotation(transformB).Rotate(_colliderStorage->GetBoxColliderOffsetPosition(_colliderB)) };

	// 位置の差
	Vector3 diff{ positionB - positionA };

	BoxBoxContactInfo info;

	info.colliderA = _colliderA;
	info.colliderB = _colliderB;

	info.depth = FLT_MAX;
	info.normal = Vector3::ZERO;
	info.axisA = 0;
	info.axisB = 0;

	// 当たり判定Aの面
	for (int i{ 0 }; i < 3; i++)
	{
		if (!CheckAxis(info, BoxBoxContactInfo::FaceA, candidateAxisA[i], diff, CalcLength(candidateAxisA[i], candidateAxisA, candidateAxisB, halfsA, halfsB), i, 0))
		{
			return false;
		}
	}
	// 当たり判定Bの面
	for (int i{ 0 }; i < 3; i++)
	{
		if (!CheckAxis(info, BoxBoxContactInfo::FaceB, candidateAxisB[i], diff, CalcLength(candidateAxisB[i], candidateAxisA, candidateAxisB, halfsA, halfsB), 0, i))
		{
			return false;
		}
	}
	// 当たり判定辺
	for (int a{ 0 }; a < 3; a++)
	{
		for (int b{ 0 }; b < 3; b++)
		{
			Vector3 axis{ Vector3::Cross(candidateAxisA[a],candidateAxisB[b])};
			// 長さが0に近い場合は飛ばす。
			if (axis.LengthSqr() <= MathConstants::EPSILON)
			{
				continue;
			}
			axis.Normalize();
			if (!CheckAxis(info, BoxBoxContactInfo::EdgeEdge, axis, diff, CalcLength(axis, candidateAxisA, candidateAxisB, halfsA, halfsB), a, b))
			{
				return false;
			}
		}
	}

	ManifoldFunction::BoxBox(
		_transformStorage,
		transformA, positionA, candidateAxisA, halfsA,
		transformB, positionB, candidateAxisB, halfsB,
		info, _manifoldBuffer);

	return true;
}

// カプセル VS カプセル
bool ContactFunction::CapsuleCapsule(ColliderID _colliderA, ColliderID _colliderB, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	const PhysicsTransformID transformA{ _colliderStorage->GetTransformID(_colliderA) };
	const PhysicsTransformID transformB{ _colliderStorage->GetTransformID(_colliderB) };

	// 中心座標
	Vector3 centers[]
	{
		_transformStorage->GetPosition(transformA) +
		_transformStorage->GetRotation(transformA).Rotate(_colliderStorage->GetCapsuleColliderOffsetPosition(_colliderA)),
		_transformStorage->GetPosition(transformB) +
		_transformStorage->GetRotation(transformB).Rotate(_colliderStorage->GetCapsuleColliderOffsetPosition(_colliderB))
	};

	// 軸の半分
	Quaternion rotA{ _transformStorage->GetRotation(transformA) * _colliderStorage->GetCapsuleColliderOffsetRotation(_colliderA) };
	Quaternion rotB{ _transformStorage->GetRotation(transformB) * _colliderStorage->GetCapsuleColliderOffsetRotation(_colliderB) };
	Vector3 halfs[]
	{
		rotA.Rotate(Vector3::UP * _colliderStorage->GetCapsuleColliderHeight(_colliderA) / 2.0f),
		rotB.Rotate(Vector3::UP * _colliderStorage->GetCapsuleColliderHeight(_colliderB) / 2.0f)
	};

	// 軸ベクトルの始点
	Vector3 starts[]
	{
		centers[0] + halfs[0],
		centers[1] + halfs[1]
	};

	// 軸ベクトルの終点
	Vector3 ends[]
	{
		centers[0] - halfs[0],
		centers[1] - halfs[1]
	};

	// 軸同士の最近点
	const auto latestPoints{ MDMath::ClosestSegmentOnSegment(starts[0],ends[0],starts[1],ends[1]) };

	// 半径の合計
	float radiusA{ _colliderStorage->GetCapsuleColliderRadius(_colliderA) };
	float radiusB{ _colliderStorage->GetCapsuleColliderRadius(_colliderB) };
	float totalRadius{ radiusA + radiusB };

	// 最近点から、最短距離を求める
	// 最近点と点の差
	Vector3 diff{ latestPoints.pointB - latestPoints.pointA };

	// 長さ
	float distSqr = diff.LengthSqr();

	// 長さが半径の合計より大きいなら当たっていない
	if (distSqr >= totalRadius * totalRadius)
	{
		return false;
	}

	Manifold manifold;
	manifold.colliderA = _colliderA;
	manifold.colliderB = _colliderB;
	if (distSqr <= MathConstants::EPSILON * MathConstants::EPSILON)
	{
		manifold.normal = Vector3::UP;
	}
	else
	{
		// 最小距離の法線
		manifold.normal = diff.Normalize();
	}
	ContactPoint contactPoint;
	// 重なり深さ計算
	contactPoint.penetration = totalRadius - sqrtf(distSqr);
	// 接触点
	Vector3 worldPointA{ latestPoints.pointA + manifold.normal * radiusA };

	Vector3 worldPointB{ latestPoints.pointB - manifold.normal * radiusB };

	contactPoint.positionLocalA = _transformStorage->GetRotation(transformA).Conjugate().Rotate(worldPointA - _transformStorage->GetPosition(transformA));

	contactPoint.positionLocalB = _transformStorage->GetRotation(transformB).Conjugate().Rotate(worldPointB - _transformStorage->GetPosition(transformB));

	// 点追加
	manifold.AddPoints(contactPoint);

	_manifoldBuffer->Add(manifold);

	return true;
}

// ボックス VS カプセル
bool ContactFunction::BoxCapsule(ColliderID _colliderBox, ColliderID _colliderCapsule, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	// TransformのDenseIndex
	PhysicsTransformID boxTransID{ _colliderStorage->GetTransformID(_colliderBox) };

	PhysicsTransformID capsuleTransID{ _colliderStorage->GetTransformID(_colliderCapsule) };

	// 中心位置
	Vector3 boxCenter{ _transformStorage->GetPosition(boxTransID) +
		_transformStorage->GetRotation(boxTransID).Rotate(_colliderStorage->GetBoxColliderOffsetPosition(_colliderBox)) };

	Vector3 capsuleCenter{ _transformStorage->GetPosition(capsuleTransID) +
		_transformStorage->GetRotation(capsuleTransID).Rotate(_colliderStorage->GetCapsuleColliderOffsetPosition(_colliderCapsule)) };

	// 回転
	Quaternion boxRot{ _transformStorage->GetRotation(boxTransID) * _colliderStorage->GetBoxColliderOffsetRotation(_colliderBox) };

	Quaternion capsuleRot{ _transformStorage->GetRotation(capsuleTransID) * _colliderStorage->GetCapsuleColliderOffsetRotation(_colliderCapsule) };

	Quaternion inverseBoxRot{ boxRot.Conjugate() };

	Quaternion inverseCapsuleRot{ capsuleRot.Conjugate() };

	// BOXのサイズ
	Vector3 boxSize{ _colliderStorage->GetBoxColliderScale(_colliderBox) };
	// BOXのサイズの半分
	Vector3 boxHalf{ boxSize * 0.5f };

	// カプセルの軸線分
	float capsuleHeight{ _colliderStorage->GetCapsuleColliderHeight(_colliderCapsule) };

	float capsuleRadius{ _colliderStorage->GetCapsuleColliderRadius(_colliderCapsule) };

	Vector3 capsuleAxisHalf{ capsuleRot.Rotate(Vector3::UP * (capsuleHeight * 0.5f)) };

	Vector3 segmentStart{ capsuleCenter + capsuleAxisHalf };

	Vector3 segmentEnd{ capsuleCenter - capsuleAxisHalf };

	/*
		カプセル軸線分とOBBの最近点を求める。

		pointSegment : カプセル軸上の最近点
		pointOBB     : Box上の最近点
	*/
	auto closest{ MDMath::ClosestPointsBetweenSegmentAndOBB(segmentStart,segmentEnd,boxCenter,boxRot,boxSize) };

	/*
		軸線分とBoxの距離がカプセル半径より大きいなら、
		カプセル表面もBoxへ届かない。
	*/
	if (closest.distanceSqr > capsuleRadius * capsuleRadius)
	{
		return false;
	}

	Manifold manifold;
	manifold.colliderA = _colliderCapsule;
	manifold.colliderB = _colliderBox;

	ContactPoint contactPoint;

	/*
		軸線分がBoxの外側にある場合。

		2つの最近点に距離があるので、
		その差から法線を作れる。
	*/
	if (closest.distanceSqr > MathConstants::EPSILON * MathConstants::EPSILON)
	{
		float distance{ std::sqrt(closest.distanceSqr) };

		/*
			カプセル軸からBoxへ向かう法線。
		*/
		manifold.normal = (closest.pointOBB - closest.pointSegment) / distance;

		// カプセル半径と最近点距離の差が貫通量
		contactPoint.penetration = capsuleRadius - distance;

		/*
			カプセル表面の接触点。

			軸上最近点からBox方向へ
			半径分進める。
		*/
		Vector3 capsuleContactWorld{ closest.pointSegment + manifold.normal * capsuleRadius };

		// Box側はOBB上の最近点がそのまま接触点
		Vector3 boxContactWorld{ closest.pointOBB };

		// ワールド座標から各Colliderのローカル座標へ変換
		contactPoint.positionLocalA =
			_transformStorage->GetRotation(capsuleTransID).Conjugate().Rotate(
			capsuleContactWorld -
			_transformStorage->GetPosition(capsuleTransID));

		contactPoint.positionLocalB =
			_transformStorage->GetRotation(boxTransID).Conjugate().Rotate(
				boxContactWorld -
				_transformStorage->GetPosition(boxTransID));
	}
	else
	{
		/*
			軸線分がBoxへ接触、または内部へ入っている場合。

			最近点同士が同じ位置になるため、
			差ベクトルから法線を作れない。

			そこで、Boxの6面から最も近い面を選び、
			その面を使って法線と接触点を作る。
		*/

		/*
			まずBox中心に最も近い軸上の点を選ぶ。

			軸がBox内部を通っている場合、
			Box内部の代表点として使える。
		*/
		Vector3 pointOnAxisWorld{ MDMath::ClosestPointOnSegment(boxCenter,segmentStart,segmentEnd) };

		Vector3 pointOnAxisLocal{ inverseBoxRot.Rotate(pointOnAxisWorld - boxCenter) };

		/*
			Box中心への最近点がBox内部にない場合がある。

			例えば、線分がBoxの角だけに接触する場合。

			その場合は、OBB最近点関数が返した
			接触点を使用する。
		*/
		bool pointInsideBox{
			pointOnAxisLocal.x >= -boxHalf.x - MathConstants::EPSILON && pointOnAxisLocal.x <= boxHalf.x + MathConstants::EPSILON &&
			pointOnAxisLocal.y >= -boxHalf.y - MathConstants::EPSILON && pointOnAxisLocal.y <= boxHalf.y + MathConstants::EPSILON &&
			pointOnAxisLocal.z >= -boxHalf.z - MathConstants::EPSILON && pointOnAxisLocal.z <= boxHalf.z + MathConstants::EPSILON
		};

		if (!pointInsideBox)
		{
			pointOnAxisWorld =closest.pointSegment;

			pointOnAxisLocal = inverseBoxRot.Rotate(pointOnAxisWorld - boxCenter);
		}

		/*
			軸上の点からBoxの各面までの距離。

			0 : -X面
			1 : +X面
			2 : -Y面
			3 : +Y面
			4 : -Z面
			5 : +Z面
		*/
		float distances[6]
		{
			pointOnAxisLocal.x + boxHalf.x,
			boxHalf.x - pointOnAxisLocal.x,

			pointOnAxisLocal.y + boxHalf.y,
			boxHalf.y - pointOnAxisLocal.y,

			pointOnAxisLocal.z + boxHalf.z,
			boxHalf.z - pointOnAxisLocal.z
		};

		/*
			浮動小数点誤差によって、
			面上の距離がわずかに負になるのを防ぐ。
		*/
		for (float& distance : distances)
		{
			distance = std::max(distance, 0.0f);
		}

		// 一番近い面を探す
		int minIndex{ 0 };

		for (int i{ 1 }; i < 6; i++)
		{
			if (distances[i] <
				distances[minIndex])
			{
				minIndex = i;
			}
		}

		/*
			CapsuleからBox内部へ向かうローカル法線。

			SolverではAが-normal方向へ押されるため、
			この向きにするとCapsuleがBoxの外へ出る。
		*/
		const Vector3 normalLocal[6]
		{
			 Vector3::RIGHT,   // -X面
			-Vector3::RIGHT,   // +X面
			 Vector3::UP,      // -Y面
			-Vector3::UP,      // +Y面
			 Vector3::FORWARD, // -Z面
			-Vector3::FORWARD  // +Z面
		};

		manifold.normal = boxRot.Rotate(normalLocal[minIndex]);

		/*
			Box面上の接触点を作る。

			軸上点をコピーし、
			選択された軸だけBox面の位置へ移動する。
		*/
		Vector3 boxPointLocal{ pointOnAxisLocal };

		switch (minIndex)
		{
		case 0:
			boxPointLocal.x = -boxHalf.x;
			break;

		case 1:
			boxPointLocal.x = boxHalf.x;
			break;

		case 2:
			boxPointLocal.y = -boxHalf.y;
			break;

		case 3:
			boxPointLocal.y = boxHalf.y;
			break;

		case 4:
			boxPointLocal.z = -boxHalf.z;
			break;

		case 5:
			boxPointLocal.z = boxHalf.z;
			break;
		}

		/*
			軸からBox面までの距離に、
			カプセル半径を足したものが貫通量。

			カプセル全体をBox外へ出すには、
			軸をBox面まで移動したあと、
			さらに半径分移動する必要がある。
		*/
		contactPoint.penetration = distances[minIndex] + capsuleRadius;

		/*
			内部時は法線がBox内部方向を向いている。

			軸上点から法線方向へ半径分進めた点を
			Capsule側接触点にする。
		*/
		Vector3 capsuleContactWorld{ pointOnAxisWorld + manifold.normal * capsuleRadius };

		Vector3 boxContactWorld{ boxCenter + boxRot.Rotate(boxPointLocal) };

		contactPoint.positionLocalA =
			_transformStorage->GetRotation(capsuleTransID).Conjugate().Rotate(
				capsuleContactWorld -
				_transformStorage->GetPosition(capsuleTransID));

		contactPoint.positionLocalB =
			_transformStorage->GetRotation(boxTransID).Conjugate().Rotate(
				boxContactWorld -
				_transformStorage->GetPosition(boxTransID));
	}

	manifold.AddPoints(contactPoint);
	_manifoldBuffer->Add(manifold);

	return true;
}

// 球 VS カプセル
bool ContactFunction::SphereCapsule(ColliderID _colliderSphere, ColliderID _colliderCapsule, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	const PhysicsTransformID sphereTransID{ _colliderStorage->GetTransformID(_colliderSphere) };
	const PhysicsTransformID capsuleTransID{ _colliderStorage->GetTransformID(_colliderCapsule) };

	// 中心座標
	Vector3 sphereCenter{ _transformStorage->GetPosition(sphereTransID) +
		_transformStorage->GetRotation(sphereTransID).Rotate(_colliderStorage->GetSphereColliderOffsetPosition(_colliderSphere)) };
	Vector3 capsuleCenter{ _transformStorage->GetPosition(capsuleTransID) +
		_transformStorage->GetRotation(capsuleTransID).Rotate(_colliderStorage->GetCapsuleColliderOffsetPosition(_colliderCapsule)) };

	// 回転
	Quaternion sphereRot{ _transformStorage->GetRotation(sphereTransID) * _colliderStorage->GetSphereColliderOffsetRotation(_colliderSphere) };
	Quaternion capsuleRot{ _transformStorage->GetRotation(capsuleTransID) * _colliderStorage->GetCapsuleColliderOffsetRotation(_colliderCapsule) };

	// 軸の半分
	Vector3 axisHalf{ capsuleRot.Rotate(Vector3::UP * _colliderStorage->GetCapsuleColliderHeight(_colliderCapsule) / 2.0f) };

	// Capsuleの始点終点
	Vector3 start{ capsuleCenter + axisHalf };
	Vector3 end{ capsuleCenter - axisHalf };

	// 最近点を求める
	Vector3 latestPoint{ MDMath::ClosestPointOnSegment(sphereCenter,start, end) };

	// 最近点と円の中心のベクトル
	Vector3 diff = latestPoint - sphereCenter;

	// 長さ
	float distSqr = diff.LengthSqr();

	// 半径の合計
	float sphereRadius{ _colliderStorage->GetSphereColliderRadius(_colliderSphere) };
	float capsuleRadius{ _colliderStorage->GetCapsuleColliderRadius(_colliderCapsule) };
	float totalRadius = sphereRadius + capsuleRadius;

	// 半径の合計が円の中心のベクトルの長さより小さいなら当たっていない
	if (distSqr > totalRadius * totalRadius)
	{
		return false;
	}

	// 衝突情報の追加
	Manifold manifold;
	manifold.colliderA = _colliderSphere;
	manifold.colliderB = _colliderCapsule;

	// 長さが0なら正規化できないので上方向にする
	if (distSqr >= MathConstants::EPSILON * MathConstants::EPSILON)
	{
		manifold.normal = diff.Normalized();
	}
	else
	{
		manifold.normal = Vector3::UP;
	}


	ContactPoint contactPoint;
	contactPoint.penetration = totalRadius - std::sqrtf(distSqr);
	// 接触点計算
	Vector3 worldPointSphere{ sphereCenter + manifold.normal * sphereRadius };

	Vector3 worldPointCapsule{ latestPoint - manifold.normal * capsuleRadius };

	contactPoint.positionLocalA = _transformStorage->GetRotation(sphereTransID).Conjugate().Rotate(
		worldPointSphere -
		_transformStorage->GetPosition(sphereTransID));

	contactPoint.positionLocalB = _transformStorage->GetRotation(capsuleTransID).Conjugate().Rotate(
		worldPointCapsule -
		_transformStorage->GetPosition(capsuleTransID));

	manifold.AddPoints(contactPoint);

	_manifoldBuffer->Add(manifold);

	return true;
}

bool ContactFunction::CheckAxis(BoxBoxContactInfo& _info, BoxBoxContactInfo::Type _type, const Vector3& _testAxis, const Vector3& _diff, float _length, int _aIndex, int _bIndex)
{
	// 分離軸候補に射影した中心間の距離
	float dist{ std::abs(Vector3::Dot(_testAxis, _diff)) };

	// 中心の距離が射影した長さより、長いなら当たっていない
	if (dist > _length)
	{
		return false;
	}

	// 衝突判定の計算
	// 重なり深さ
	float depth = _length - dist;
	// 辺を優先しないための補正
	float edgeSLot{ (_type == BoxBoxContactInfo::EdgeEdge) ? 0.1f : 0.0f };
	// 今の重なり深さより短いなら代入
	if (depth < _info.depth + edgeSLot)
	{
		_info.depth = depth;
		_info.normal = (Vector3::Dot(_testAxis, _diff) < 0.0f)
			? -_testAxis
			: _testAxis;
		switch (_type)
		{
		case BoxBoxContactInfo::FaceA:
			_info.axisA = _aIndex;
			break;
		case BoxBoxContactInfo::FaceB:
			_info.axisB = _bIndex;
			break;
		case BoxBoxContactInfo::EdgeEdge:
			_info.axisA = _aIndex;
			_info.axisB = _bIndex;
			break;
		default:
			break;
		}
		_info.type = _type;
	}
	return true;
}

float ContactFunction::CalcLength(const Vector3& _axis, const Vector3* _candidateAxisA, const Vector3* _candidateAxisB, const float* _halfsA, const float* _halfsB)
{
	float result{ 0 };
	// 長さの計算
	for (int j = 0; j < 3; j++)
	{
		// 分離軸候補に射影した長さを足していく
		result += std::abs(Vector3::Dot(_axis, _candidateAxisA[j])) * _halfsA[j];
		result += std::abs(Vector3::Dot(_axis, _candidateAxisB[j])) * _halfsB[j];
	}

	return result;
}
