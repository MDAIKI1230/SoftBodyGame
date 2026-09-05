#pragma once

#include <vector>

#include "EndPoint.h"
#include "ConstraintTuning.h"

struct DistanceConstraint
{
public:
	// EndPoint削除処理
	void RemoveEndpoint(PhysicsTransformID _transformID)
	{
		for (int i{ 0 }; i < endPoints.size(); i++)
		{
			// 同じIDがあったら削除
			if (endPoints[i].transformID == _transformID)
			{
				endPoints[i] = endPoints.back();
				endPoints.pop_back();

				return;
			}
		}
	}
public:
	// 拘束のメンバー
	std::vector<EndPoint> endPoints;
	// 距離
	float distance{ 0.0f };

	// 柔らかさなどの調整用数値
	ConstraintTuning tuning;
};
