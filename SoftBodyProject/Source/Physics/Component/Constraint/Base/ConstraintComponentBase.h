#pragma once

#include "MDMath.h"

#include "EndPointFrame.h"

#include "ConstraintID.h"
#include "EntityID.h"

struct ConstraintComponentBase
{
public:
	// コンストラクタ
	ConstraintComponentBase(ConstraintID _id) :
		id{ _id }
	{
	}

	// 自信のEndPoint取得
	const EndPointFrame& GetEndPoint();
	// 自信のEndPoint変更
	void SetEndPoint(const EndPointFrame& _endPoint);

	// 対応点追加
	void AddEndPoint(EntityID _entityID, const Vector3& _localOffset, const Quaternion& _rotation = Quaternion::Identity());
	// 対応点削除
	void RemoveEndPoint(EntityID _entityID);

	// ID取得
	ConstraintID GetID() const { return id; }
protected:
	ConstraintID id;
};
