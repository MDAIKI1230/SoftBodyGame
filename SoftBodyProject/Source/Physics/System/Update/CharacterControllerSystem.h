#pragma once

#include "CharacterControllerStorage.h"
#include "PhysicsTransformStorage.h"
#include "BodyStorage.h"
#include "ColliderStorage.h"

class CharacterControllerSystem
{
public:
	// 更新関数
	void FixedUpdate(CharacterControllerStorage* _characterControllerStorage, PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage);

private:
	// 地面との状態を更新する関数
	void UpdateGroundState(CharacterControllerStorage* _characterControllerStorage, PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage);
	// 移動更新関数
	void UpdateMovement(CharacterControllerStorage* _characterControllerStorage, BodyStorage* _bodyStorage);
	// ジャンプリクエスト処理関数
	void UpdateJump(CharacterControllerStorage* _characterControllerStorage, BodyStorage* _bodyStorage);

	// 空中にいる時の移動更新関数
	void UpdateAirboneState(uint32_t _indexCC, CharacterControllerStorage* _characterControllerStorage, BodyStorage* _bodyStorage, float _deltaTime);
	// 歩ける状態の時の移動更新関数
	void UpdateWalkableState(uint32_t _indexCC, CharacterControllerStorage* _characterControllerStorage, BodyStorage* _bodyStorage, float _deltaTime);
	// 滑る地面の上にいる時の移動更新関数
	void UpdateSteepSlopeState(uint32_t _indexCC, CharacterControllerStorage* _characterControllerStorage, BodyStorage* _bodyStorage, float _deltaTime);
};
