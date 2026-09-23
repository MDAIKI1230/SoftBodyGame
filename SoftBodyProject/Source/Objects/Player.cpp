#include "ResourceManager.h"
#include "InputSystem.h"

#include "ActiveRagdollComponent.h"
#include "AnimationComponent.h"
#include "RendererComponent.h"

#include "Player.h"

// コンストラクタ
Player::Player(WorldStorage* _world, EntityID _entityID, Camera* _camera) :
	ObjectBase(_world, _entityID),
	camera{ _camera }
{
	AddComponent<TransformComponent>();

	GetComponent<TransformComponent>()->SetPosition(Vector3{ 200.0f,0,0 });

	InputSystem::GetInputAction("Character", "Move").AddPerformedCallback<&Player::Move>(this);
	InputSystem::GetInputAction("Character", "Move").AddCanceledCallback<&Player::Move>(this);
	InputSystem::GetInputAction("Character", "Jump").AddPerformedCallback<&Player::Jump>(this);
	InputSystem::GetInputAction("Camera", "LookMouse").AddPerformedCallback<&Player::CameraMove>(this);

	cc = AddComponent<CharacterControllerComponent>();
	cc->SetGroundDeceleration(2000.0f);
	cc->SetJumpSpeed(100.0f);

	camera->GetComponent<CameraRigComponent>()->SetMode(CameraMode::TPS);
	camera->GetComponent<CameraRigComponent>()->SetFollowTarget(GetID());
	camera->GetComponent<CameraComponent>()->SetNear(0.01f);

	/*RendererComponent* renderer{ AddComponent<RendererComponent>(ResourceManager::GetModel("M_001_player_095_01_no_sword_walk.mv1")) };
	AddComponent<ActiveRagdollComponent>(*renderer, "Res/Data/Ragdoll/Active/M_001_player_095_01_no_sword.json");
	AnimationComponent* anim{ AddComponent<AnimationComponent>(renderer) };
	anim->SetAnimationName("Walk");
	anim->Play();
	anim->SetLoop(true);*/
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
	Vector2 input{ _input.ReadValue<Vector2>() };
	Vector3 local{ camera->GetComponent<TransformComponent>()->GetRotation().Rotate(Vector3{ input.x,0.0f,input.y }) };
	cc->SetMoveInput(local);
}
void Player::Stop(InputActionContext _input)
{
	cc->ClearMoveInput();
}
void Player::Jump(InputActionContext _input)
{
	if (_input.ReadValue<bool>())
	{
		cc->RequestJump();
	}
}
void Player::CameraMove(InputActionContext _input)
{
	camera->GetComponent<CameraRigComponent>()->AddLookDelta(_input.ReadValue<Vector2>() * 0.01f);
}