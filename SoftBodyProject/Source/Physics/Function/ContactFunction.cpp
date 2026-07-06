#include "ContactFunction.h"

bool ContactFunction::BoxBox(const ColliderID& _colliderIDA, const ColliderID& _colliderIDB, ColliderStorage* _colliderStorage, PhysicsTransformStorage* _transformStorage, CollisionManifoldBuffer* _manifoldBuffer)
{
	// Aの情報取得
	Vector3 halfScaleA{ _colliderStorage->boxStorage->scale[_colliderStorage->GetDenseIndex(_colliderIDA)] * 0.5f };
	uint32_t transformIndexA{ _transformStorage->GetDenseIndex(_colliderStorage->GetTransformID(_colliderIDA)) };
	Quaternion rotA{ _transformStorage->rotation[transformIndexA] };
	// Bの情報取得
	Vector3 halfScaleB{ _colliderStorage->boxStorage->scale[_colliderStorage->GetDenseIndex(_colliderIDB)] * 0.5f };
	uint32_t transformIndexB{ _transformStorage->GetDenseIndex(_colliderStorage->GetTransformID(_colliderIDB)) };
	Quaternion rotB{ _transformStorage->rotation[transformIndexB] };

	// スケールの適応
	halfScaleA = SIMDVectorMath::Mul(halfScaleA, _transformStorage->scale[transformIndexA]);

	halfScaleB = SIMDVectorMath::Mul(halfScaleB, _transformStorage->scale[transformIndexB]);

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
	Vector3 diff{ _transformStorage->position[transformIndexB] - _transformStorage->position[transformIndexA] };

	BoxBoxContactInfo info;

	info.colliderA = _colliderIDA;
	info.colliderB = _colliderIDB;

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

	ManifoldFunction::BoxBox(_transformStorage->position[transformIndexA], _transformStorage->position[transformIndexB], candidateAxisA, candidateAxisB, halfsA, halfsB, info, _manifoldBuffer);

	return true;
}

bool ContactFunction::CheckAxis(BoxBoxContactInfo& _info, BoxBoxContactInfo::Type _type, Vector3& _testAxis, Vector3& _diff, float _length, int _aIndex, int _bIndex)
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

float ContactFunction::CalcLength(Vector3& _axis,Vector3* _candidateAxisA, Vector3* _candidateAxisB,float* _halfsA, float* _halfsB)
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
