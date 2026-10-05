#include "ResourceManager.h"
#include "InputSystem.h"
#include "GameManager.h"
#include "Renderer.h"

#include "AnimationComponent.h"
#include "RendererComponent.h"
#include "ActiveRagdollComponent.h"
#include "CharacterControllerComponent.h"

#include "Player.h"

// コンストラクタ
Player::Player(WorldStorage* _world, EntityID _entityID, Camera* _camera) :
	ObjectBase(_world, _entityID),
	camera{ _camera },
	HAND_LIMIT_ANGLE_RAD{ MDMath::DegToRad(HAND_LIMIT_ANGLE_DEG) }
{
	AddComponent<TransformComponent>();

	//GetComponent<TransformComponent>()->SetPosition(Vector3{ 0.0f,200.0f,-300.0f });
	GetComponent<TransformComponent>()->SetPosition(Vector3{ 0.0f,200.0f,0.0f });

	InputSystem::GetInputAction("Character", "Move").AddPerformedCallback<&Player::Move>(this);
	InputSystem::GetInputAction("Character", "Move").AddCanceledCallback<&Player::Stop>(this);
	InputSystem::GetInputAction("Character", "Jump").AddPerformedCallback<&Player::Jump>(this);
	InputSystem::GetInputAction("Character", "GrabLeft").AddPerformedCallback <&Player::RaiseLeftHand>(this);
	InputSystem::GetInputAction("Character", "GrabLeft").AddCanceledCallback <&Player::RaiseLeftHandEnd>(this);
	InputSystem::GetInputAction("Character", "GrabRight").AddPerformedCallback <&Player::RaiseRightHand>(this);
	InputSystem::GetInputAction("Character", "GrabRight").AddCanceledCallback <&Player::RaiseRightHandEnd>(this);
	InputSystem::GetInputAction("Camera", "LookMouse").AddPerformedCallback<&Player::CameraMoveMouse>(this);
	InputSystem::GetInputAction("Camera", "LookGamePad").AddPerformedCallback <&Player::CameraMovePad>(this);

	camera->GetComponent<CameraRigComponent>()->SetMode(CameraMode::TPS);
	camera->GetComponent<CameraRigComponent>()->SetFollowTarget(GetID());
	camera->GetComponent<CameraComponent>()->SetNear(0.1f);
	camera->GetComponent<CameraComponent>()->SetFar(2000.0f);

	RendererComponent* renderer{ AddComponent<RendererComponent>(
		ResourceManager::GetModel("M_001_player_095_01_no_sword_walk_high_knee.mv1")
	) };

	ActiveRagdollComponent* active{ AddComponent<ActiveRagdollComponent>(
		*renderer, "Res/Data/Ragdoll/Active/M_001_player_095_01_no_sword.json"
	) };

	AnimationComponent* anim{ AddComponent<AnimationComponent>(
		renderer,"Res/Data/Skeleton/M_001_player_095_01_no_sword_LegsBoneMask.json"
	) };
	anim->SetAnimationName("Walk");
	anim->SetLoop(true);

	camera->GetComponent<CameraRigComponent>()->GetRayFilter(active->GetIgnoreFilter());

	CharacterControllerComponent* cc{ AddComponent<CharacterControllerComponent>() };
	cc->SetCollisionFilter(active->GetIgnoreFilter());
	cc->SetColliderHeight(50.0f);
	cc->SetColliderOffset(Vector3{ 0.0f,50.0f,0.0f });

	active->SetRotationWeight(RagdollBoneRole::TORSO, 0.2f);
	active->SetPositionWeight(RagdollBoneRole::TORSO, 0.2f);
}

// --- 更新系 ---

// 更新処理
void Player::Update()
{
}
// 物理更新処理
void Player::FixedUpdate()
{

}

// --- 衝突系 ---

// 衝突始め
void Player::OnCollisionEnter()
{

}
void Player::OnCollision()
{

}
// 衝突終わり
void Player::OnCollisionExit()
{

}

void Player::Move(InputActionContext _input)
{
	if (!GameManager::IsScene())
	{
		return;
	}

	Vector2 input{ _input.ReadValue<Vector2>() };
	Vector3 local{ camera->GetComponent<TransformComponent>()->GetRotation().Rotate(Vector3{ input.x,0.0f,input.y }) };
	GetComponent<CharacterControllerComponent>()->SetMoveInput(local);
	GetComponent<AnimationComponent>()->Play();
	GetComponent<AnimationComponent>()->SetActive(true);
}
void Player::Stop(InputActionContext _input)
{
	GetComponent<CharacterControllerComponent>()->ClearMoveInput();
	GetComponent<AnimationComponent>()->Stop();
	GetComponent<AnimationComponent>()->SetTime(0.0f);
	GetComponent<AnimationComponent>()->SetActive(false);
}
void Player::Jump(InputActionContext _input)
{
	if (!GameManager::IsScene())
	{
		return;
	}

	if (_input.ReadValue<bool>())
	{
		GetComponent<CharacterControllerComponent>()->RequestJump();
	}
}
void Player::CameraMoveMouse(InputActionContext _input)
{
	if (!GameManager::IsScene())
	{
		return;
	}

	camera->GetComponent<CameraRigComponent>()->AddLookDelta(_input.ReadValue<Vector2>() * 0.01f);
}
void Player::CameraMovePad(InputActionContext _input)
{
	if (!GameManager::IsScene())
	{
		return;
	}

	Vector2 value{ _input.ReadValue<Vector2>() };
	value = { value.x,-value.y };
	camera->GetComponent<CameraRigComponent>()->AddLookDelta(value * 0.1f);
}
void Player::RaiseLeftHand(InputActionContext _input)
{
	if (!GameManager::IsScene())
	{
		return;
	}

	ActiveRagdollComponent* activeRagdoll{ GetComponent<ActiveRagdollComponent>() };

	// 肩の位置取得
	Vector3 shoulderPosition;
	if (!activeRagdoll->GetBoneWorldPosition(RagdollBoneRole::LEFT_UPPER_ARM, shoulderPosition))
	{
		return;
	}

	// カメラの回転を取得
	TransformComponent* cameraTransform{ camera->GetComponent<TransformComponent>() };
	const Quaternion& cameraRot{ cameraTransform->GetRotation() };
	// 前方向の取得
	Quaternion forwardRotate{ CalcHandForwardRotation() };

	Vector3 handPosition{ shoulderPosition +
		forwardRotate.Rotate(Vector3::FORWARD * HAND_DISTANCE) +
		cameraRot.Rotate(HAND_SIDE_OFFSET) };

	StartActiveRagdollHand(activeRagdoll, RagdollBoneRole::LEFT_HAND, handPosition);

	canLeftHandGrab = true;
}
void Player::RaiseLeftHandEnd(InputActionContext _input)
{
	StopActiveRagdollHand(RagdollBoneRole::LEFT_HAND);
	canLeftHandGrab = false;
}
void Player::RaiseRightHand(InputActionContext _input)
{
	if (!GameManager::IsScene())
	{
		return;
	}

	ActiveRagdollComponent* activeRagdoll{ GetComponent<ActiveRagdollComponent>() };

	// 肩の位置取得
	Vector3 shoulderPosition;
	if (!activeRagdoll->GetBoneWorldPosition(RagdollBoneRole::RIGHT_UPPER_ARM, shoulderPosition))
	{
		return;
	}

	// カメラの回転を取得
	TransformComponent* cameraTransform{ camera->GetComponent<TransformComponent>() };
	const Quaternion& cameraRot{ cameraTransform->GetRotation() };
	// 前方向の取得
	Quaternion forwardRotate{ CalcHandForwardRotation()};

	Vector3 handPosition{ shoulderPosition +
		forwardRotate.Rotate(Vector3::FORWARD * HAND_DISTANCE) -
		cameraRot.Rotate(HAND_SIDE_OFFSET) };

	StartActiveRagdollHand(activeRagdoll, RagdollBoneRole::RIGHT_HAND, handPosition);

	canRightHandGrab = true;
}
void Player::RaiseRightHandEnd(InputActionContext _input)
{
	StopActiveRagdollHand(RagdollBoneRole::RIGHT_HAND);
	canRightHandGrab = false;
}

// 手の位置を計算する関数
Quaternion Player::CalcHandForwardRotation()
{
	CameraRigComponent* cameraRig{ camera->GetComponent<CameraRigComponent>() };
	// 回転が行き過ぎないようにクランプ
	float handPitch{ std::clamp(cameraRig->GetPitch() - MDMath::DegToRad(CAMERA_ROTATION_OFFSET), -HAND_LIMIT_ANGLE_RAD, HAND_LIMIT_ANGLE_RAD) };
	return Quaternion::Euler(handPitch, cameraRig->GetYaw(), 0.0f);
}
// 手の動きに対してウェイトを設定する関数
void Player::StartActiveRagdollHand(ActiveRagdollComponent* _activeRagdoll, RagdollBoneRole _handRole, const Vector3 _handPosition)
{
	_activeRagdoll->SetTargetPosition(_handRole, _handPosition);
	_activeRagdoll->SetPositionWeight(_handRole, HAND_GRAB_WEIGHT);
}
// 手の動きを解除する際のウェイトの設定関数
void Player::StopActiveRagdollHand(RagdollBoneRole _handRole)
{
	ActiveRagdollComponent* activeRagdoll{ GetComponent<ActiveRagdollComponent>() };
	activeRagdoll->ClearTargetPosition(_handRole);
	activeRagdoll->SetPositionWeight(_handRole, 0.0f);
}