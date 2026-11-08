#pragma once

#include "CharacterControllerStorage.h"
#include "PhysicsTransformStorage.h"
#include "BodyStorage.h"
#include "ColliderStorage.h"
#include "ConstraintStorage.h"

#include "RayCastQueryHitInfo.h"

class CharacterControllerSystem
{
public:
	// 位置積分前更新関数
	void PreIntegralUpdate(CharacterControllerStorage* _characterControllerStorage, PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage);
	// 拘束ソルバ前更新関数
	void PreSolverUpdate(CharacterControllerStorage* _characterControllerStorage, PhysicsTransformStorage* _transformStorage, ConstraintStorage* _constraintStorage);

private:
	// 地面との状態を更新する関数
	void UpdateGroundState(CharacterControllerStorage* _characterControllerStorage, PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage);

	// 情報のリセット
	void ResetInfo(CharacterControllerStorage* _characterControllerStorage, CharacterControllerID _id);
	// 情報のセット
	void SetInfo(CharacterControllerStorage* _characterControllerStorage, CharacterControllerID _id, RayCastQueryHitInfo _info, CharacterGroundState _state, const Vector3& _rayOffset);
	// ジャンプ状態中の判定
	CharacterGroundState CheckJumping(CharacterGroundState _pastState, CharacterGroundState _defaultState, const Vector3& _velocity, const Vector3& _groundNormal);

	// ジャンプリクエスト処理関数
	void UpdateJump(CharacterControllerStorage* _characterControllerStorage, BodyStorage* _bodyStorage);

	// 拘束の条件の更新
	void UpdateConstraint(CharacterControllerStorage* _characterControllerStorage, ConstraintStorage* _constraintStorage, PhysicsTransformStorage* _transformStorage);
	// 速度拘束による移動更新関数
	void UpdateMotor(CharacterControllerStorage* _characterControllerStorage, BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage);
	// 目標速度の作成関数
	Vector3 CreateTargetVelocity(
		ColliderStorage* _colliderStorage, BodyStorage* _bodyStorage,
		const Vector3& _input,
		const RayCastQueryHitInfo& _groundRayHitInfo, const Vector3& _velocity,
		const float _maxSpeed, const float _acc, const float _dec,
		const float _scale, const float _deltaTime);
	// 入力方向の目標速度を満たすための力追加
	void AddMoverForce(
		Vector3& _force,
		const Vector3& _targetVelocity, const Vector3& _velocity,
		const Vector3& _gravity, const Vector3& _groundNormal,
		const float _inverseMass, const float _mass,
		const float _maxForce, const float _deltaTime);
	// 落下速度を制御する
	void ControlGravity(Vector3& _force, const Vector3& _velocity, const Vector3& _gravity, const float _inverseMass, const float _mass, const float _maxSpeed, const float _deltaTime);
};