#pragma once

#include "EndPointFrame.h"
#include "ConstraintTuning.h"

struct AngleLimitPointConstraint
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
		endPoints.clear();
	}
public:
	EndPointFrame ownerEndPoint;
	// 拘束のメンバー
	std::vector<EndPointFrame> endPoints;

	// 制限角度
	float angleMax{ 0.0f };
	float angleMin{ 0.0f };

	// 柔らかさなどの調整用数値
	ConstraintTuning tuning;
};