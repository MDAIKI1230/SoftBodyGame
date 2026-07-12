#pragma once

#include "ComponentBase.h"

#include "MDMath.h"

#include "EntityID.h"

#include "ConstraintID.h"

struct DistanceConstraintComponent : public ComponentBase
{
public:
	// コンストラクタ
	DistanceConstraintComponent(EntityID _entity, int _handle);
	// コンストラクタ
	DistanceConstraintComponent(EntityID _entity, int _handle, Vector3 _localOffset);
	// コンストラクタ
	DistanceConstraintComponent(EntityID _entity, int _handle, float _distance);
	// コンストラクタ
	DistanceConstraintComponent(EntityID _entity, int _handle, Vector3 _localOffset, float _distance);

	// 対応点追加
	void AddEndPoint(const EntityID& _entityID, const Vector3& _localOffset);

	// 距離取得
	float GetDistance();
	// 距離変更
	void SetDistance(float _distance);
private:
	ConstraintID id;
};
