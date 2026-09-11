#pragma once

#include "DirectionEndPoint.h"
#include "ConstraintTuning.h"

struct AngleLimitPointConstraint
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
	// 拘束のメンバー
	std::vector<DirectionEndPoint> directionEndPoints;

	// 制限角度
	float angleMax{ 0.0f };
	float angleMin{ 0.0f };

	// 柔らかさなどの調整用数値
	ConstraintTuning tuning;
};