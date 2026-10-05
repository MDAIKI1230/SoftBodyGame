#pragma once

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
	void CameraMoveMouse(InputActionContext _input);
	void CameraMovePad(InputActionContext _input);
	void RaiseLeftHand(InputActionContext _input);
	void RaiseLeftHandEnd(InputActionContext _input);
	void RaiseRightHand(InputActionContext _input);
	void RaiseRightHandEnd(InputActionContext _input);
private:
	static constexpr float HAND_DISTANCE{ 25.0f };
	static constexpr float HAND_LIMIT_ANGLE_DEG{ 85.0f };
	const float HAND_LIMIT_ANGLE_RAD;
	const Vector3 HAND_SIDE_OFFSET{ 20.0f,0.0f,0.0f };
	const float CAMERA_ROTATION_OFFSET{ 20.0f };
private:
	Camera* camera;

	InputAction cameraMove;
};
