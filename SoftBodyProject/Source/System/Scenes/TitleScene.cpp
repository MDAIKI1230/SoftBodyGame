#include <memory>

#include "ResourceManager.h"
#include "InputSystem.h"
#include "SoundPlayer.h"

#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

#include "EmptyObject.h"
#include "Title/PressToStart.h"
#include "Escape/Escape.h"
#include "Escape/ResumeUI.h"
#include "Escape/QuitUI.h"
#include "Escape/Check.h"

#include "ApplicationRequest.h"

#include "TitleScene.h"

// コンストラクタ
TitleScene::TitleScene()
{

}

// 初期化
void TitleScene::Initialize()
{
	ResourceManager::LoadTexture("Res/Texture/TitleBG.png");
	ResourceManager::LoadTexture("Res/Texture/TitleUI.png");
	ResourceManager::LoadTexture("Res/Texture/PressToStart.png");
	ResourceManager::LoadSound("Res/Sound/BGM/TitleBGM.wav");
	ResourceManager::LoadSound("Res/Sound/SE/Confirm.wav");
	ResourceManager::LoadSound("Res/Sound/SE/Escape.wav");
	ResourceManager::LoadSound("Res/Sound/SE/Select.wav");
	ResourceManager::LoadSound("Res/Sound/SE/PressToStart.wav");

	SoundPlayInfo info;
	info.loop = true;
	info.volume = 0.5f;

	SoundPlayer::Play(ResourceManager::GetSound("TitleBGM.wav"), info);

	InputSystem::LoadAsset("Res/Data/Input/TitleInput.json");

	InputSystem::GetInputAction("Title", "PressToStart").AddStartedCallback<&TitleScene::ToGameScene>(this);

	std::unique_ptr<EmptyObject> bg{ std::make_unique<EmptyObject>(&worldStorage, objectManager.GenerateNewID()) };
	bg->AddComponent<SpriteRendererComponent>(ResourceManager::GetTexture("TitleBG.png"));

	objectManager.Add(std::move(bg));

	std::unique_ptr<EmptyObject> ui{ std::make_unique<EmptyObject>(&worldStorage, objectManager.GenerateNewID()) };
	ui->AddComponent<SpriteRendererComponent>(ResourceManager::GetTexture("TitleUI.png"));
	ui->GetComponent<TransformComponent>()->SetPosition(Vector3{ 100.0f,50.0f,0.0f });

	objectManager.Add(std::move(ui));

	objectManager.Add(std::make_unique<PressToStart>(&worldStorage, objectManager.GenerateNewID()));

	std::unique_ptr<Escape> escape{ std::make_unique<Escape>(&worldStorage, objectManager.GenerateNewID()) };

	Escape* escapePtr{ escape.get() };

	objectManager.Add(std::move(escape));

	std::unique_ptr<ResumeUI> resume{ std::make_unique<ResumeUI>(&worldStorage, objectManager.GenerateNewID()) };
	escapePtr->AddSlecte(resume.get());

	objectManager.Add(std::move(resume));

	std::unique_ptr<QuitUI> quit{ std::make_unique<QuitUI>(&worldStorage, objectManager.GenerateNewID()) };
	escapePtr->AddSlecte(quit.get());

	objectManager.Add(std::move(quit));

	std::unique_ptr<Check> check{ std::make_unique<Check>(&worldStorage, objectManager.GenerateNewID()) };
	escapePtr->SetCheck(check.get());

	objectManager.Add(std::move(check));
}

void TitleScene::ToGameScene(InputActionContext _input)
{
	SoundPlayInfo info;
	info.loop = true;
	info.volume = 0.5f;

	SoundPlayer::Play(ResourceManager::GetSound("PressToStart.wav"), info);

	ApplicationRequest::SceneRequest(SceneRequestType::TUTORIAL);
}