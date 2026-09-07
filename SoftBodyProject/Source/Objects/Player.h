#pragma once

#include "CharacterControllerComponent.h"

#include "InputActionContext.h"
#include "InputAction.h"

#include "ObjectBase.h"

#include "Camera.h"

class Player :public ObjectBase
{
public:
	// コンストラクタ
	Player(WorldStorage* _world, EntityID _entityID, Camera* _camera);

	// --- 更新系 ---

	// 更新処理
	void Update() override;
	// 物理更新処理
	void FixedUpdate() override;

	// --- 衝突系 ---

	// 衝突始め
	void OnCollisionEnter() override;
	// 衝突中ずっと
	 void OnCollision() override;
	// 衝突終わり
	void OnCollisionExit() override;
private:
	void Move(InputActionContext _input);
	void Stop(InputActionContext _input);
	void Jump(InputActionContext _input);
	void CameraMove(InputActionContext _input);
private:
	CharacterControllerComponent* cc;
	Camera* camera;

	InputAction cameraMove;
};
