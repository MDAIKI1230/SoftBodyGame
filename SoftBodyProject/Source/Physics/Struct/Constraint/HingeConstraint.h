#pragma once

#include "DirectionEndPoint.h"
#include "ConstraintTuning.h"

struct HingeConstraint
{
public:
	// EndPoint削除処理
	void RemoveEndpoint(PhysicsTransformID _transformID)
	{
		for (int i{ 0 }; i < directionEndPoints.size(); i++)
		{
			// 同じIDがあったら削除
			if (directionEndPoints[i].transformID == _transformID)
			{
				directionEndPoints[i] = directionEndPoints.back();
				directionEndPoints.pop_back();

				return;
			}
		}
	}
public:
	// 自身の情報
	DirectionEndPoint ownerEndPoint;

	// 所有者へ接続されるBody
	std::vector<DirectionEndPoint> directionEndPoints;

	// 柔らかさなどの調整用数値(角度と位置で別に用意)
	ConstraintTuning positionTuning;
	ConstraintTuning angularTuning;
};
