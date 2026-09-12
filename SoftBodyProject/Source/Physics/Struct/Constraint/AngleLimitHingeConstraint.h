#pragma once

#include "AngleLimitHingeEndPoint.h"
#include "ConstraintTuning.h"

struct AngleLimitHingeConstraint
{
public:
	// EndPoint削除処理
	void RemoveEndpoint(PhysicsTransformID _transformID)
	{
		for (int i{ 0 }; i < angleLimitHingeEndPoints.size(); i++)
		{
			// 同じIDがあったら削除
			if (angleLimitHingeEndPoints[i].transformID == _transformID)
			{
				angleLimitHingeEndPoints[i] = angleLimitHingeEndPoints.back();
				angleLimitHingeEndPoints.pop_back();

				return;
			}
		}
	}
public:
	// 自身の情報
	AngleLimitHingeEndPoint ownerEndPoint;
	// 拘束のメンバー
	std::vector<AngleLimitHingeEndPoint> angleLimitHingeEndPoints;

	// 制限角度
	float angleMax{ 0.0f };
	float angleMin{ 0.0f };

	// 柔らかさなどの調整用数値(角度と位置で別に用意)
	ConstraintTuning positionTuning;
	ConstraintTuning angularTuning;
};