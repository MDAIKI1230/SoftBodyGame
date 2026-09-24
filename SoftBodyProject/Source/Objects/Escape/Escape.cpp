#include "ResourceManager.h"
#include "InputSystem.h"
#include "GameManager.h"
#include "SoundPlayer.h"

#include "BaseConstants.h"

#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

#include "Escape.h"

// コンストラクタ
Escape::Escape(WorldStorage* _world, EntityID _entityID) :
	ObjectBase{ _world,_entityID }
{
	ResourceManager::LoadTexture("Res/Texture/Escape.png");

	AddComponent<SpriteRendererComponent>(ResourceManager::GetTexture("Escape.png"));

	GetComponent<TransformComponent>()->SetPosition(BASE_POS);
	GetComponent<TransformComponent>()->TranslateWorld(Vector3::UP * Config::WINDOW_SIZE_H);

	InputSystem::GetInputAction("Escape", "Toggle").AddStartedCallback<&Escape::EscapeInput>(this);
	InputSystem::GetInputAction("Escape", "SelectUp").AddStartedCallback<&Escape::SlectedUp>(this);
	InputSystem::GetInputAction("Escape", "SelectDown").AddStartedCallback<&Escape::SlectedDown>(this);
	InputSystem::GetInputAction("Escape", "Navigate").AddStartedCallback<&Escape::SlectedStick>(this);
	InputSystem::GetInputAction("Escape", "Confirm").AddStartedCallback<&Escape::Action>(this);
}

// 更新関数
void Escape::Update()
{
	TransformComponent* trans{ GetComponent<TransformComponent>() };
	const Vector3& position{ trans->GetPosition() };

	if (GameManager::IsScene() && position.y < Config::WINDOW_SIZE_H)
	{
		trans->SetPosition(position + Vector3::UP * speed);
		if (position.y >= Config::WINDOW_SIZE_H)
		{
			trans->SetPosition(Vector3{ position.x,Config::WINDOW_SIZE_H,position.z });
			// 隠れ切ったところで選択インデックスをリセットしておく
			selectPartIndex = 0;
		}

		ReCalcChild();
	}
	else if (GameManager::IsEscape() && position.y > BASE_POS.y)
	{
		trans->SetPosition(trans->GetPosition() - Vector3::UP * speed);
		if (position.y < BASE_POS.y)
		{
			trans->SetPosition(Vector3{ position.x,BASE_POS.y,position.z });
		}

		ReCalcChild();
	}
}

// エスケープパーツの選択肢追加
void Escape::AddSlecte(EscapePart* _parts)
{
	// 位置をオフセットで調整からの追加
	Vector3 position{ GetComponent<TransformComponent>()->GetPosition() };
	position += POSITION_OFFSET + (PARTS_COUNT_OFFSET * static_cast<float>(parts.size()));
	_parts->GetComponent<TransformComponent>()->SetPosition(position);
	parts.push_back(_parts);
}

// チェックマークセット
void Escape::SetCheck(Check* _check)
{
	check = _check;
	ReCalcCheckPos();
}

void Escape::EscapeInput(InputActionContext& _input)
{
	if (GameManager::IsScene())
	{
		GameManager::StartEscape();
	}
	else if(GameManager::IsEscape())
	{
		GameManager::StartScene();
	}

	SoundPlayer::Play(ResourceManager::GetSound("Escape.wav"), soundInfo);

	_input.Consume();
}

void Escape::SlectedUp(InputActionContext& _input)
{
	if (!GameManager::IsEscape() || parts.empty())
	{
		return;
	}

	int size{ static_cast<int>(parts.size()) };
	selectPartIndex =
		(selectPartIndex + size - 1) % size;
	ReCalcCheckPos();

	SoundPlayer::Play(ResourceManager::GetSound("Select.wav"), soundInfo);

	_input.Consume();
}

void Escape::SlectedDown(InputActionContext& _input)
{
	if (!GameManager::IsEscape() || parts.empty())
	{
		return;
	}

	++selectPartIndex %= parts.size();
	ReCalcCheckPos();

	SoundPlayer::Play(ResourceManager::GetSound("Select.wav"), soundInfo);

	_input.Consume();
}

void Escape::SlectedStick(InputActionContext& _input)
{
	if (!GameManager::IsEscape() || parts.empty())
	{
		return;
	}

	Vector2 value{ _input.ReadValue<Vector2>() };

	// 横方向の入力が強い場合は、上下操作として扱わない
	if (std::abs(value.y) <= std::abs(value.x))
	{
		return;
	}


	if (value.y > 0.0f)
	{
		int size{ static_cast<int>(parts.size()) };
		selectPartIndex =
			(selectPartIndex + size - 1) % size;
		ReCalcCheckPos();

		SoundPlayer::Play(ResourceManager::GetSound("Select.wav"), soundInfo);
	}
	else if (value.y < 0.0f)
	{
		++selectPartIndex %= parts.size();
		ReCalcCheckPos();

		SoundPlayer::Play(ResourceManager::GetSound("Select.wav"), soundInfo);
	}

	_input.Consume();
}

void Escape::Action(InputActionContext& _input)
{
	if (!GameManager::IsEscape() || parts.empty())
	{
		return;
	}

	SoundPlayer::Play(ResourceManager::GetSound("Confirm.wav"), soundInfo);

	parts[selectPartIndex]->Action();

	_input.Consume();
}

void Escape::ReCalcCheckPos()
{
	Vector3 partPosition{ parts[selectPartIndex]->GetComponent<TransformComponent>()->GetPosition() };
	check->GetComponent<TransformComponent>()->SetPosition(partPosition + CHECK_OFFSET);
}

void Escape::ReCalcChild()
{
	// 追加時と同じ計算
	Vector3 position{ GetComponent<TransformComponent>()->GetPosition() };
	
	for (int i{ 0 }; i < parts.size(); i++)
	{
		EscapePart* part{ parts[i] };
		part->GetComponent<TransformComponent>()->SetPosition(
			position +
			POSITION_OFFSET + (PARTS_COUNT_OFFSET * static_cast<float>(i)));
	}

	ReCalcCheckPos();
}