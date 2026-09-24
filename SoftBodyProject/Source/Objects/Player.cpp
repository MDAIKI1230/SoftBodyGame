#include "ResourceManager.h"
#include "InputSystem.h"

#include "AnimationComponent.h"
#include "RendererComponent.h"

#include "Player.h"

// コンストラクタ
Player::Player(WorldStorage* _world, EntityID _entityID, Camera* _camera) :
	ObjectBase(_world, _entityID),
	camera{ _camera }
{
	AddComponent<TransformComponent>();

	GetComponent<TransformComponent>()->SetPosition(Vector3{ 0.0f,200,200 });

	InputSystem::GetInputAction("Character", "Move").AddPerformedCallback<&Player::Move>(this);
	InputSystem::GetInputAction("Character", "Move").AddCanceledCallback<&Player::Stop>(this);
	InputSystem::GetInputAction("Character", "Jump").AddPerformedCallback<&Player::Jump>(this);
	InputSystem::GetInputAction("Camera", "LookMouse").AddPerformedCallback<&Player::CameraMove>(this);

	camera->GetComponent<CameraRigComponent>()->SetMode(CameraMode::TPS);
	camera->GetComponent<CameraRigComponent>()->SetFollowTarget(GetID());
	camera->GetComponent<CameraComponent>()->SetNear(0.1f);
	camera->GetComponent<CameraComponent>()->SetFar(2000.0f);

	RendererComponent* renderer{ AddComponent<RendererComponent>(ResourceManager::GetModel("M_001_player_095_01_no_sword_walk_high.mv1")) };
	ActiveRagdollComponent* active{ AddComponent<ActiveRagdollComponent>(*renderer, "Res/Data/Ragdoll/Active/M_001_player_095_01_no_sword.json") };
	AnimationComponent* anim{ AddComponent<AnimationComponent>(renderer,"Res/Data/Skeleton/M_001_player_095_01_no_sword_LegsBoneMask.json") };
	anim->SetAnimationName("Walk");
	anim->SetLoop(true);

	camera->GetComponent<CameraRigComponent>()->GetRayFilter(active->GetIgnoreFilter());
}

// --- 更新系 ---

// 更新処理
void Player::Update()
{
	// GetComponent<AnimationComponent>()->Play();
}
// 物理更新処理
void Player::FixedUpdate()
{
	ActiveRagdollComponent* active{ GetComponent<ActiveRagdollComponent>() };

	GetComponent<TransformComponent>()->SetPosition(active->GetBody(RagdollBoneRole::PELVIS).GetPosition());
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
	Vector2 input{ _input.ReadValue<Vector2>() };
	Vector3 local{ camera->GetComponent<TransformComponent>()->GetRotation().Rotate(Vector3{ input.x,0.0f,input.y }) };
	GetComponent<ActiveRagdollComponent>()->SetMoveInput(local);
	GetComponent<AnimationComponent>()->Play();
}
void Player::Stop(InputActionContext _input)
{
	GetComponent<ActiveRagdollComponent>()->ClearMoveInput();
	GetComponent<AnimationComponent>()->Stop();
}
void Player::Jump(InputActionContext _input)
{
	if (_input.ReadValue<bool>())
	{
		GetComponent<ActiveRagdollComponent>()->RequestJump();
	}
}
void Player::CameraMove(InputActionContext _input)
{
	camera->GetComponent<CameraRigComponent>()->AddLookDelta(_input.ReadValue<Vector2>() * 0.01f);
}