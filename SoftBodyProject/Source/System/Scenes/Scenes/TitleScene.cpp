#include <memory>

#include "ResourceManager.h"
#include "GameManager.h"
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

#include "LoadingScreen/FadeOutLoadingScreen.h"

#include "TitleScene.h"

// コンストラクタ
TitleScene::TitleScene()
{

}

// 初期化
void TitleScene::Initialize()
{
	GameManager::StartScene();

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

	EmptyObject* bg{ Create<EmptyObject>() };
	bg->AddComponent<SpriteRendererComponent>(ResourceManager::GetTexture("TitleBG.png"));

	EmptyObject* ui{ Create<EmptyObject>() };
	ui->AddComponent<SpriteRendererComponent>(ResourceManager::GetTexture("TitleUI.png"));
	ui->GetComponent<TransformComponent>()->SetPosition(Vector3{ 100.0f,50.0f,0.0f });

	Create<PressToStart>();

	Escape* escape{ Create<Escape>() };

	Escape* escapePtr{ escape };

	ResumeUI* resume{ Create<ResumeUI>() };
	escapePtr->AddSlecte(resume);

	QuitUI* quit{ Create<QuitUI>() };
	escapePtr->AddSlecte(quit);

	Check* check{ Create<Check>() };
	escapePtr->SetCheck(check);
}

void TitleScene::ToGameScene(InputActionContext _input)
{
	SoundPlayInfo info;
	info.loop = true;
	info.volume = 0.5f;

	SoundPlayer::Play(ResourceManager::GetSound("PressToStart.wav"), info);

	ApplicationRequest::SceneRequest<"DebugScene">(std::make_unique<FadeOutLoadingScreen>());
}