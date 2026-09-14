#pragma once

#include "EndPointFrame.h"
#include "ConstraintTuning.h"

/*
	角度付きの点拘束に、軸に対しての角度制限もある。
*/
struct LimitedBallJointConstraint
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

	// 所有者へ接続されるBody
	std::vector<EndPointFrame> endPoints;

	// Twist制限角度
	float twistAngle;
	// Swing制限角度
	float swingAngle;

	// 柔らかさなどの調整用数値(角度と位置で別に用意)
	ConstraintTuning positionTuning;
	ConstraintTuning angularTuning;
};