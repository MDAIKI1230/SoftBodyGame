#pragma once

#include "AngleLimitHingeEndPoint.h"
#include "ConstraintTuning.h"

struct AngleLimitHingeConstraint
{
public:
	// EndPoint削除処理
	void RemoveEndpoint(PhysicsTransformID _transformID)
	{
		for (int i{ 0 }; i < endPoint.size(); i++)
		{
			// 同じIDがあったら削除
			if (endPoint[i].transformID == _transformID)
			{
				endPoint[i] = endPoint.back();
				endPoint.pop_back();

				return;
			}
		}
	}
public:
	// 自身の情報
	AngleLimitHingeEndPoint ownerEndPoint;
	// 拘束のメンバー
	std::vector<AngleLimitHingeEndPoint> endPoint;

	// 制限角度の内積値
	float angleMaxDot{ 0.0f };
	float angleMinDot{ 0.0f };

	// 柔らかさなどの調整用数値(角度と位置で別に用意)
	ConstraintTuning positionTuning;
	ConstraintTuning angularTuning;
};