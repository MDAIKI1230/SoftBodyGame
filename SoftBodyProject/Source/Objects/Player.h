#pragma once

#include "InputActionContext.h"

#include "ActiveRagdollComponent.h"
#include "PointConstraintComponent.h"

#include "MonoBehaviour.h"

#include "Camera.h"

class Player :public MonoBehaviour
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
	void OnCollisionEnter(CollisionInfo _info) override;
	// 衝突中ずっと
	 void OnCollision(CollisionInfo _info) override;
	// 衝突終わり
	void OnCollisionExit(CollisionInfo _info) override;
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
	// 手の位置を計算する関数
	Quaternion CalcHandForwardRotation();
	// 手の動きに対してウェイトを設定する関数
	void StartActiveRagdollHand(ActiveRagdollComponent* _activeRagdoll, RagdollBoneRole _handRole, const Vector3 _handPosition);
	// 手の動きを解除する際のウェイトの設定関数
	void StopActiveRagdollHand(RagdollBoneRole _handRole);
private:
	static constexpr float HAND_DISTANCE{ 25.0f };
	static constexpr float HAND_LIMIT_ANGLE_DEG{ 85.0f };
	static constexpr float HAND_GRAB_WEIGHT{ 0.2f };
	const float HAND_LIMIT_ANGLE_RAD;
	const Vector3 HAND_SIDE_OFFSET{ 20.0f,0.0f,0.0f };
	const float CAMERA_ROTATION_OFFSET{ 20.0f };
private:
	Camera* camera;

	bool canLeftHandGrab{ false };
	bool canRightHandGrab{ false };

	PointConstraintComponent leftHandConstraint;
	PointConstraintComponent rightHandConstraint;

	ColliderComponent leftHandCollider;
	ColliderComponent rightHandCollider;
};
