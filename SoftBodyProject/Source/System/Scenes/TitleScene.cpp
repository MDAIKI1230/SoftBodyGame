#include <memory>

#include "ResourceManager.h"
#include "InputSystem.h"

#include "SpriteRendererComponent.h"
#include "TransformComponent.h"

#include "EmptyObject.h"
#include "Title/PressToStart.h"
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

	InputSystem::LoadAsset("Res/Data/Input/TitleInput.json");

	InputSystem::GetInputAction("Title", "PressToStart").AddStartedCallback<&TitleScene::ToGameScene>(this);
	InputSystem::GetInputAction("Title", "Exit").AddStartedCallback<&TitleScene::Escape>(this);

	std::unique_ptr<EmptyObject> bg{ std::make_unique<EmptyObject>(&worldStorage, objectManager.GenerateNewID()) };
	bg->AddComponent<SpriteRendererComponent>(ResourceManager::GetTexture("TitleBG.png"));

	objectManager.Add(std::move(bg));

	std::unique_ptr<EmptyObject> ui{ std::make_unique<EmptyObject>(&worldStorage, objectManager.GenerateNewID()) };
	ui->AddComponent<SpriteRendererComponent>(ResourceManager::GetTexture("TitleUI.png"));
	ui->GetComponent<TransformComponent>()->SetPosition(Vector3{ 100.0f,50.0f,0.0f });

	objectManager.Add(std::move(ui));

	objectManager.Add(std::make_unique<PressToStart>(&worldStorage, objectManager.GenerateNewID()));
}

void TitleScene::Escape(InputActionContext _input)
{
	ApplicationRequest::ExitRequest();
}
void TitleScene::ToGameScene(InputActionContext _input)
{
	ApplicationRequest::SceneRequest(SceneRequestType::GAME);
}