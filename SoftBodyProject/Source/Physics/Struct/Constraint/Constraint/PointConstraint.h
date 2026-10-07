#pragma once

#include <vector>

#include "EndPointFrame.h"
#include "ConstraintTuning.h"

struct PointConstraint
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
	// EndPoint自分以外削除処理
	void RemoveEndpointOtherAll()
	{
		if (endPoints.size() <= 1)
		{
			return;
		}
		while (true)
		{
			endPoints.pop_back();
			if (endPoints.size() == 1)
			{
				return;
			}
		}
	}
public:
	// 拘束のメンバー
	std::vector<EndPointFrame> endPoints;

	// 柔らかさなどの調整用数値
	ConstraintTuning tuning;
};
