#pragma once

#include "EndPointFrame.h"

struct ConstraintBase
{
	// EndPoint削除処理
	void RemoveEndPoint(PhysicsTransformID _transformID)
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
	// 自身の情報
	EndPointFrame ownerEndPoint;
	// 拘束のメンバー
	std::vector<EndPointFrame> endPoints;
};