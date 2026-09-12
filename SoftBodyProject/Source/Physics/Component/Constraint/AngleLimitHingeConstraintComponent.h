#pragma once

#include "MDMath.h"

#include "EntityID.h"
#include "ConstraintID.h"
#include "ConstraintTuning.h"

struct AngleLimitHingeConstraintComponent
{
public:
	// コンストラクタ
	AngleLimitHingeConstraintComponent(EntityID _entity);
	// コンストラクタ
	AngleLimitHingeConstraintComponent(EntityID _entity, Vector3 _localOffset);
	// コンストラクタ
	AngleLimitHingeConstraintComponent(EntityID _entity, float _angleMin, float _angleMax);
	// コンストラクタ
	AngleLimitHingeConstraintComponent(EntityID _entity, Vector3 _localOffset, Vector3 _localAxis, Vector3 _localDirection);
	// コンストラクタ
	AngleLimitHingeConstraintComponent(EntityID _entity, Vector3 _localOffset, Vector3 _localAxis, Vector3 _localDirection, float _angleMin, float _angleMax);

	// 対応点追加
	void AddEndPoint(EntityID _entityID, const Vector3& _localOffset, const Vector3& _localAxis, const Vector3& _localDirection);
	// 対応点削除
	void RemoveEndPoint(EntityID _entityID);

	// 最小角度取得
	float GetAngleMin() const;
	// 最小角度変更
	void SetAngleMin(float _angleMin);

	// 最大角度取得
	float GetAngleMax() const;
	// 最大角度変更
	void SetAngleMax(float _angleMax);

	// 角度範囲変更
	void SetAngleRange(float _angleMin, float _angleMax);

	// 位置Tuning取得
	ConstraintTuning GetPositionTuning() const;
	// 位置Tuning変更
	void SetPositionTuning(const ConstraintTuning& _tuning);

	// 回転Tuning取得
	ConstraintTuning GetAngularTuning() const;
	// 回転Tuning変更
	void SetAngularTuning(const ConstraintTuning& _tuning);

	// ID取得
	ConstraintID GetID() const { return id; }
private:
	ConstraintID id;
};