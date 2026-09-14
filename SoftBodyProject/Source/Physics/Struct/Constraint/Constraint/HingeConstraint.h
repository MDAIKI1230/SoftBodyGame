#pragma once

#include "EndPointFrame.h"
#include "ConstraintTuning.h"

/*
	コンポーネントの所有者自身が基盤となる軸を持ち
	追加された奴らは、その軸を元に拘束をされる
*/
struct HingeConstraint
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

	// 柔らかさなどの調整用数値(角度と位置で別に用意)
	ConstraintTuning positionTuning;
	ConstraintTuning angularTuning;
};
