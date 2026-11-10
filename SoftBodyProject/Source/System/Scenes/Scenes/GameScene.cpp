#include "ResourceManager.h"
#include "GameManager.h"
#include "InputSystem.h"

#include "Escape/Escape.h"
#include "Escape/ResumeUI.h"
#include "Escape/QuitUI.h"
#include "Escape/Check.h"
#include "Camera.h"

#include "ApplicationRequest.h"

#include "GameScene.h"

// コンストラクタ
GameScene::GameScene()
{

}

// 初期化
void GameScene::Initialize()
{
	GameManager::StartScene();

	InputSystem::LoadAsset("Res/Data/Input/CharacterInput.json");

	ResourceManager::LoadTexture("Res/Texture/TitleBG.png");
	ResourceManager::LoadTexture("Res/Texture/TitleUI.png");
	ResourceManager::LoadTexture("Res/Texture/PressToStart.png");
	ResourceManager::LoadSound("Res/Sound/BGM/TitleBGM.wav");
	ResourceManager::LoadSound("Res/Sound/SE/Confirm.wav");
	ResourceManager::LoadSound("Res/Sound/SE/Escape.wav");
	ResourceManager::LoadSound("Res/Sound/SE/Select.wav");
	ResourceManager::LoadSound("Res/Sound/SE/PressToStart.wav");

	ResourceManager::LoadModel("Res/Model/Stage/BedroomFoundation.mv1");
	ResourceManager::LoadModel("Res/Model/M_001_player_095_01.mv1");

	LoadFile("Res/Data/Scene/GameScene.json");

	Camera* camera{ Create<Camera>() };
	ResourceManager::LoadCubeTexture("Res/Texture/Sky/NaturalDayMeadow_Cubemap.dds");
	camera->GetComponent<CameraComponent>()->SetSkyTextureHandle(ResourceManager::GetCubeTexture("NaturalDayMeadow_Cubemap.dds"));
	camera->GetComponent<CameraComponent>()->SetClearMode(ClearMode::SKY);
	camera->GetComponent<CameraComponent>()->SetNear(1.0f);
	camera->GetComponent<CameraComponent>()->SetFar(5000.0f);
	camera->GetComponent<CameraComponent>()->SetIsMain(true);
	Camera::main = camera;

	Escape* escape{ Create<Escape>() };

	Escape* escapePtr{ escape };

	ResumeUI* resume{ Create<ResumeUI>() };
	escapePtr->AddSlecte(resume);

	QuitUI* quit{ Create<QuitUI>() };
	escapePtr->AddSlecte(quit);

	Check* check{ Create<Check>() };
	escapePtr->SetCheck(check);
}