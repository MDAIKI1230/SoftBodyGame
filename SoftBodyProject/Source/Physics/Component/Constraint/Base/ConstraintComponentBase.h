#pragma once

#include "MDMath.h"

#include "EndPointFrame.h"

#include "ConstraintID.h"
#include "EntityID.h"

#include "RigidBodyComponent.h"

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
	void AddEndPoint(EntityID _entityID, const Vector3& _localOffset, const Quaternion& _rotation = Quaternion::IDENTITY);
	// 対応点削除
	void RemoveEndPoint(EntityID _entityID);

	// Bodyを指定して対応点を追加
	void AddEndPoint(const RigidBodyComponent& _body, const Vector3& _localOffset, const Quaternion& _rotation = Quaternion::IDENTITY);

	// Bodyを指定して対応点を削除
	void RemoveEndPoint(const RigidBodyComponent& _body);

	// ID取得
	ConstraintID GetID() const { return id; }
protected:
	ConstraintID id;
};
