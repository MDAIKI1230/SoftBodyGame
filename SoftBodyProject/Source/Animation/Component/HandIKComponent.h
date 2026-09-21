#pragma once

#include <string_view>

#include "EntityID.h"
#include "FeatureIKID.h"

#include "MDMath.h"
#include "RendererComponent.h"

struct HandIKComponent
{
	friend class HandIKComponentStorage;
public:
	// コンストラクタ
	HandIKComponent(EntityID _entity, const RendererComponent* _renderer);

	// 上腕ボーン取得
	std::string_view GetUpperArm();
	// 上腕ボーン変更
	void SetUpperArm(const char* _boneName);

	// 前腕ボーン取得
	std::string_view GetLowerArm();
	// 前腕ボーン変更
	void SetLowerArm(const char* _boneName);

	// 手ボーン取得
	std::string_view GetHand();
	// 手ボーン変更
	void SetHand(const char* _boneName);

	// ターゲット位置取得
	Vector3 GetTargetPosition();
	// ターゲット位置変更
	void SetTargetPosition(const Vector3& _targetPosition);

	// ターゲット回転取得
	Quaternion GetTargetRotation();
	// ターゲット回転変更
	void SetTargetRotation(const Quaternion& _targetRotation);

	// ポール位置取得
	Vector3 GetPolePosition();
	// ポール位置変更
	void SetPolePosition(const Vector3& _polePosition);

	// 位置ウェイト取得
	float GetPositionWeight();
	// 位置ウェイト変更
	void SetPositionWeight(float _positionWeight);

	// 回転ウェイト取得
	float GetRotationWeight();
	// 回転ウェイト変更
	void SetRotationWeight(float _rotationWeight);
private:
	FeatureIKID id;
};