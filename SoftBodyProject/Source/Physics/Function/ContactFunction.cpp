#include <algorithm>

#include "ContactFunction.h"

bool ContactFunction::SphereSphere(ColliderID _colliderA, ColliderID _colliderB, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	// Aの情報取得
	float radiusA{ _colliderStorage->sphereStorage->radius[_colliderStorage->GetDenseIndex(_colliderA)] };
	uint32_t transformIndexA{ _transformStorage->GetDenseIndex(_colliderStorage->GetTransformID(_colliderA)) };
	// Bの情報取得
	float radiusB{ _colliderStorage->sphereStorage->radius[_colliderStorage->GetDenseIndex(_colliderB)] };
	uint32_t transformIndexB{ _transformStorage->GetDenseIndex(_colliderStorage->GetTransformID(_colliderB)) };

	// スケールの適応(書く方向で最大を選ぶ)
	Vector3 scale{ _transformStorage->GetScale(transformIndexA) };
	radiusA *= std::max(std::max(scale.x, scale.y), scale.z);

	scale = _transformStorage->GetScale(transformIndexB);
	radiusB *= std::max(std::max(scale.x, scale.y), scale.z);

	// 半径の合計
	float totalRadius{ radiusA + radiusB };

	// 差
	Vector3 diff{ _transformStorage->GetPosition(transformIndexB) - _transformStorage->GetPosition(transformIndexA) };

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
		contactPoint.positionLocalA = _transformStorage->GetRotation(transformIndexA).Conjugate().Rotate(manifold.normal * radiusA);
		contactPoint.positionLocalB = _transformStorage->GetRotation(transformIndexB).Conjugate().Rotate(-manifold.normal * radiusB);

		manifold.AddPoints(contactPoint);

		_manifoldBuffer->manifolds.push_back(manifold);

		return true;
	}

	return false;
}

bool ContactFunction::SphereBox(ColliderID _colliderSphere, ColliderID _colliderBox, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	// Sphereの情報取得
	float radius{ _colliderStorage->sphereStorage->radius[_colliderStorage->GetDenseIndex(_colliderSphere)] };
	uint32_t transformIndexSphere{ _transformStorage->GetDenseIndex(_colliderStorage->GetTransformID(_colliderSphere)) };
	// Boxの情報取得
	Vector3 halfScaleBox{ _colliderStorage->boxStorage->scale[_colliderStorage->GetDenseIndex(_colliderBox)] * 0.5f };
	uint32_t transformIndexBox{ _transformStorage->GetDenseIndex(_colliderStorage->GetTransformID(_colliderBox)) };
	Quaternion rotBox{ _transformStorage->GetRotation(transformIndexBox) };

	halfScaleBox = SIMDVectorMath::Mul(halfScaleBox, _transformStorage->GetScale(transformIndexBox));

	// 基底ベクトル
	Vector3 axis[]
	{
		rotBox.Rotate(Vector3::RIGHT),
		rotBox.Rotate(Vector3::UP),
		rotBox.Rotate(Vector3::FORWARD)
	};

	// ローカル座標
	Vector3 temp{ _transformStorage->GetPosition(transformIndexSphere) - _transformStorage->GetPosition(transformIndexBox) };
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

		// クランプしたのをワールドに直すして衝突点にする(BOX)
		contactPoint.positionLocalA = _transformStorage->GetRotation(transformIndexSphere).Conjugate().Rotate(manifold.normal * radius);
		// 球は法線から求める
		contactPoint.positionLocalB = _transformStorage->GetRotation(transformIndexBox).Conjugate().Rotate(axis[0] * latestPoint.x + axis[1] * latestPoint.y + axis[2] * latestPoint.z);

		// 点追加
		manifold.AddPoints(contactPoint);

		_manifoldBuffer->manifolds.push_back(manifold);

		return true;
	}

	// 中心点が矩形の中

	// 4辺との距離
	float distances[6]
	{
	localCirclePosition.x - halfScaleBox.x,  // left
	halfScaleBox.x - localCirclePosition.x,  // right
	localCirclePosition.y - halfScaleBox.y,  // bottom
	halfScaleBox.y - localCirclePosition.y,  // top
	localCirclePosition.z - halfScaleBox.z,  // front
	halfScaleBox.z - localCirclePosition.z   // back
	};

	// 対応した法線
	Vector3 normals[6]
	{
	axis[0],  // left
	-axis[0], // right
	-axis[1], // bottom
	axis[1],  // top
	axis[2],  // front
	-axis[2]  // back
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
	contactPoint.penetration = distances[minIndex];
	// クランプしたのをワールドに直すして衝突点にする(BOX)
	contactPoint.positionLocalA = _transformStorage->GetRotation(transformIndexSphere).Conjugate().Rotate(manifold.normal * radius);
	// 球は法線から求める
	contactPoint.positionLocalB = _transformStorage->GetRotation(transformIndexBox).Conjugate().Rotate(axis[0] * latestPoint.x + axis[1] * latestPoint.y + axis[2] * latestPoint.z);

	// 点追加
	manifold.AddPoints(contactPoint);

	_manifoldBuffer->manifolds.push_back(manifold);

	return true;
}

bool ContactFunction::BoxBox(ColliderID _colliderA, ColliderID _colliderB, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	// Aの情報取得
	Vector3 halfScaleA{ _colliderStorage->boxStorage->scale[_colliderStorage->GetDenseIndex(_colliderA)] * 0.5f };
	uint32_t transformIndexA{ _transformStorage->GetDenseIndex(_colliderStorage->GetTransformID(_colliderA)) };
	Quaternion rotA{ _transformStorage->GetRotation(transformIndexA) };
	// Bの情報取得
	Vector3 halfScaleB{ _colliderStorage->boxStorage->scale[_colliderStorage->GetDenseIndex(_colliderB)] * 0.5f };
	uint32_t transformIndexB{ _transformStorage->GetDenseIndex(_colliderStorage->GetTransformID(_colliderB)) };
	Quaternion rotB{ _transformStorage->GetRotation(transformIndexB) };

	// スケールの適応
	halfScaleA = SIMDVectorMath::Mul(halfScaleA, _transformStorage->GetScale(transformIndexA));

	halfScaleB = SIMDVectorMath::Mul(halfScaleB, _transformStorage->GetScale(transformIndexB));

	// 分離軸候補
	Vector3 candidateAxisA[]
	{
		rotA.Rotate(Vector3::RIGHT),
		rotA.Rotate(Vector3::UP),
		rotA.Rotate(Vector3::FORWARD),
	};

	Vector3 candidateAxisB[]
	{
		rotB.Rotate(Vector3::RIGHT),
		rotB.Rotate(Vector3::UP),
		rotB.Rotate(Vector3::FORWARD),
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

	// 位置の差
	Vector3 diff{ _transformStorage->GetPosition(transformIndexB) - _transformStorage->GetPosition(transformIndexA) };

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
		_transformStorage->GetPosition(transformIndexA), _transformStorage->GetRotation(transformIndexA), candidateAxisA, halfsA,
		_transformStorage->GetPosition(transformIndexB), _transformStorage->GetRotation(transformIndexB), candidateAxisB, halfsB,
		info, _manifoldBuffer);

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
