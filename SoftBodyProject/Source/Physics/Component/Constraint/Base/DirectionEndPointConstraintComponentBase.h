#pragma once

#include "MDMath.h"

#include "EntityID.h"
#include "ConstraintID.h"

struct DirectionEndPointConstraintComponentBase
{
public:
	// IDを渡すコンストラクタ
	DirectionEndPointConstraintComponentBase(ConstraintID _id) :
		id{ _id }
	{
	}

	// 対応点追加
	void AddEndPoint(EntityID _entityID, const Vector3& _localOffset, const Vector3& _localDirection);
	// 対応点削除
	void RemoveEndPoint(EntityID _entityID);
protected:
	ConstraintID id;
};