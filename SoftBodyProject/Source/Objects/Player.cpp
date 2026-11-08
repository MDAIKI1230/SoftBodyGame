#include "ResourceManager.h"
#include "InputSystem.h"
#include "GameManager.h"
#include "TimeManager.h"

#include "AnimationComponent.h"
#include "ModelRendererComponent.h"
#include "ActiveRagdollComponent.h"
#include "CharacterControllerComponent.h"
#include "CapsuleColliderComponent.h"

#include "Player.h"

// コンストラクタ
Player::Player(WorldStorage* _world, EntityID _entityID) :
	MonoBehaviour{_world, _entityID},
	HAND_LIMIT_ANGLE_RAD{ MDMath::DegToRad(HAND_LIMIT_ANGLE_DEG) },
	leftHandConstraint{ _entityID },
	rightHandConstraint{ _entityID }
{
}

Player::Player(WorldStorage* _world, EntityID _entityID, Camera* _camera) :
	MonoBehaviour{_world, _entityID},
	camera{ camera },
	HAND_LIMIT_ANGLE_RAD{ MDMath::DegToRad(HAND_LIMIT_ANGLE_DEG) },
	leftHandConstraint{ _entityID },
	rightHandConstraint{ _entityID }
{
}

void Player::Start()
{
	camera = Camera::main;

	if (!camera)
	{
		return;
	}

	//GetComponent<TransformComponent>()->SetPosition(Vector3{ 0.0f,200.0f,-300.0f });
	CapsuleColliderComponent* collider{ AddComponent<CapsuleColliderComponent>(50.0f, 25.0f) };
	collider->SetOffsetPosition(Vector3::UP * 50.0f);
	CollisionFilter testFilter;
	testFilter.categoryBits = CollisionTag::Type::CARRYABLE;
	collider->SetFilter(testFilter);
	GetComponent<TransformComponent>()->SetPosition(Vector3{ 150.0f,200.0f,0.0f });

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
	camera->GetComponent<CameraRigComponent>()->SetPivotOffset(CHARACTER_OFFSET);
	camera->GetComponent<CameraComponent>()->SetNear(0.1f);
	camera->GetComponent<CameraComponent>()->SetFar(2000.0f);

	ModelRendererComponent* renderer{ AddComponent<ModelRendererComponent>(
		ResourceManager::GetModel("M_001_player_095_01.mv1")
	) };

	/*ActiveRagdollComponent* active{ AddComponent<ActiveRagdollComponent>(
		*renderer, "Res/Data/Ragdoll/Active/M_001_player_095_01_no_sword.json"
	) };*/

	AnimationComponent* anim{ AddComponent<AnimationComponent>(
		renderer,"Res/Data/Skeleton/M_001_player_095_01_no_sword_LegsBoneMask.json"
	) };
	anim->SetAnimationName("Walk");
	anim->SetLoop(true);

	//CollisionFilter ragdollFilter{ active->GetIgnoreFilter() };
	testFilter.IgnoreCategory(CollisionTag::Type::CARRYABLE);
	camera->GetComponent<CameraRigComponent>()->GetRayFilter(testFilter);
	// collider->SetFilter(ragdollFilter);

	CharacterControllerComponent* cc{ AddComponent<CharacterControllerComponent>(*AddComponent<RigidBodyComponent>()) };
	cc->SetCollisionFilter(testFilter);
	cc->SetGroundProbeDistance(5.0f);
	cc->SetSkirtRadius(25.0f);
	cc->SetSkirtRayCount(8);
	cc->SetMaxJumpTime(2.0f);
	cc->SetMaxMoverForceSlopeAngleDeg(45.0f);

	/*active->SetRotationWeight(RagdollBoneRole::TORSO, 0.2f);
	active->SetPositionWeight(RagdollBoneRole::TORSO, 0.2f);*/

	/*RigidBodyComponent leftHandBody{ active->GetBody(RagdollBoneRole::LEFT_HAND) };
	leftHandConstraint = *AddComponent<PointConstraintComponent>(leftHandBody);

	RigidBodyComponent rightHandBody{ active->GetBody(RagdollBoneRole::RIGHT_HAND) };
	rightHandConstraint = *AddComponent<PointConstraintComponent>(rightHandBody);

	leftHandCollider = active->GetCollider(RagdollBoneRole::LEFT_HAND);
	rightHandCollider = active->GetCollider(RagdollBoneRole::RIGHT_HAND);

	ignoreActiveRagdoll = active->GetIgnoreFilter();*/
}

// --- 更新系 ---

// 更新処理
void Player::Update()
{
	CharacterGroundState state{ GetComponent<CharacterControllerComponent>()->GetGroundState() };

	if (state == CharacterGroundState::WALKABLE || state == CharacterGroundState::JUMPING)
	{
		//GetComponent<ActiveRagdollComponent>()->SetControlState(ActiveRagdollControlState::STANDING);
	}
	else
	{
		//GetComponent<ActiveRagdollComponent>()->SetControlState(ActiveRagdollControlState::AIRBORNE);
	}
}
// 物理更新処理
void Player::FixedUpdate()
{
	CharacterControllerComponent* cc{ GetComponent<CharacterControllerComponent>() };

	Quaternion rotation{ cc->GetRotation() };
	Vector3 velocity{ cc->GetVelocity() };

	Vector3 nowForward{ rotation.Rotate(Vector3::FORWARD) };

	nowForward = SIMDVectorMath::Mul(nowForward, Vector3::FORWARD + Vector3::RIGHT);

	if (nowForward.LengthSqr() <= MathConstants::EPSILON)
	{
		return;
	}

	Vector3 targetForward{ velocity.Normalized() };

	targetForward = SIMDVectorMath::Mul(targetForward, Vector3::FORWARD + Vector3::RIGHT);

	if (targetForward.LengthSqr() <= MathConstants::EPSILON)
	{
		return;
	}

	float theta{ Vector3::SignedAngleNormal(nowForward,targetForward,Vector3::UP) };

	if (fabs(theta) <= MathConstants::EPSILON)
	{
		return;
	}

	float maxTheta{ ANGULER_VELOCITY * TimeManager::GetFixedDeltaTime() };

	theta = std::clamp(theta, -maxTheta, maxTheta);

	Quaternion deltaRot{ Quaternion::AngleAxis(theta,Vector3::UP) };

	cc->Rotate(deltaRot);
}

// --- 衝突系 ---

// 衝突始め
void Player::OnCollisionEnter(CollisionInfo _info)
{
	if (_info.otherCollider.CompareGroup(ignoreActiveRagdoll))
	{
		return;
	}

	if (canLeftHandGrab)
	{
		if (_info.selfCollider == leftHandCollider)
		{
			EndPointFrame endPoint;
			endPoint.weight = 0.0f;
			leftHandConstraint.SetEndPoint(endPoint);
			leftHandConstraint.AddEndPoint(_info.other, _info.otherContact.contactLocalPosition);
		}
	}

	if (canRightHandGrab)
	{
		if (_info.selfCollider == rightHandCollider)
		{
			EndPointFrame endPoint;
			endPoint.weight = 0.0f;
			rightHandConstraint.SetEndPoint(endPoint);
			rightHandConstraint.AddEndPoint(_info.other, _info.otherContact.contactLocalPosition);
		}
	}
}
void Player::OnCollision(CollisionInfo _info)
{

}
// 衝突終わり
void Player::OnCollisionExit(CollisionInfo _info)
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
	GetComponent<AnimationComponent>()->SetAnimationName("Walk");
	GetComponent<AnimationComponent>()->Play();
	GetComponent<AnimationComponent>()->SetActive(true);
}
void Player::Stop(InputActionContext _input)
{
	GetComponent<CharacterControllerComponent>()->ClearMoveInput();
	GetComponent<AnimationComponent>()->StartCrossFadeToBindPose(1.0f);
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
	leftHandConstraint.RemoveEndPointOtherAll();
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
	rightHandConstraint.RemoveEndPointOtherAll();
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