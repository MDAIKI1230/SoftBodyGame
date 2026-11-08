#pragma once

#include "EndPointFrame.h"

struct ConstraintOneBase
{
public:
	// EndPoint削除処理
	void RemoveEndPoint()
	{
		otherEndPoint = {};
	}
public:
	// コンポーネントそのものポイント
	EndPointFrame ownerEndPoint;
	// 相手
	EndPointFrame otherEndPoint;
};