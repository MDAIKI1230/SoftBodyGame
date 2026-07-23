#pragma once

#include "MDMath.h"

#include "ConstraintID.h"
#include "EntityID.h"

struct EndPointConstraintComponentBase
{
public:
	// コンストラクタ
	EndPointConstraintComponentBase(const ConstraintID& _id) :
		id{ _id }
	{
	}

	// 対応点追加
	void AddEndPoint(EntityID& _entityID, const Vector3& _localOffset);
	// 対応点削除
	void RemoveEndPoint(EntityID& _entityID);
protected:
	ConstraintID id;
};
