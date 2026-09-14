#pragma once

#include "EndPointFrame.h"
#include "ConstraintTuning.h"

struct AngleLimitHingeConstraint
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
	// 自身の情報
	EndPointFrame ownerEndPoint;
	// 拘束のメンバー
	std::vector<EndPointFrame> endPoints;

	// 制限角度
	float angleMax{ 0.0f };
	float angleMin{ 0.0f };

	// 柔らかさなどの調整用数値(角度と位置で別に用意)
	ConstraintTuning positionTuning;
	ConstraintTuning angularTuning;
};