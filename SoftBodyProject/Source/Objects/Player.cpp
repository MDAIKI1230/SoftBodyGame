#include "ServiceLocator.h"

#include "Player.h"

// コンストラクタ
Player::Player(WorldStorage* _world, EntityID _entityID):
	ObjectBase(_world,_entityID)
{
	AddComponent<TransformComponent>();
	cc = AddComponent<CharacterControllerComponent>();

	GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,500,0 });

	ServiceLocator::GetInputSytem()->GetInputAction("Character","Move").AddPerformedCallback<&Player::Move>(this);
	ServiceLocator::GetInputSytem()->GetInputAction("Character","Move").AddCanceledCallback<&Player::Move>(this);
	ServiceLocator::GetInputSytem()->GetInputAction("Character","Jump").AddPerformedCallback<&Player::Jump>(this);

	cc->SetGroundDeceleration(2000.0f);
	cc->SetJumpSpeed(100.0f);
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
	cc->SetMoveInput(Vector3{ input.x,0.0f,input.y });
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
