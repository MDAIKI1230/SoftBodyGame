#include "InputSystem.h"
#include "ResourceManager.h"
#include "SoundPlayer.h"
#include "Renderer.h"

#include "Camera.h"
#include "Player.h"
#include "TutorialBox.h"
#include "Escape/Escape.h"
#include "Escape/ResumeUI.h"
#include "Escape/TitleUI.h"
#include "Escape/QuitUI.h"
#include "Escape/Check.h"

#include "TutorialScene.h"

// 初期化
void TutorialScene::Initialize()
{
	InputSystem::LoadAsset("Res/Data/Input/CharacterInput.json");
	ResourceManager::LoadModel("Res/Model/M_001_player_095_01_no_sword_walk_high_knee.mv1");
	ResourceManager::LoadModel("Res/Model/MovableStageRiser.mv1");
	LoadFile("Res/Data/Scene/ExhibitionTutorialStageSceneData.json");
	ResourceManager::LoadSound("Res/Sound/BGM/TutorialBGM.wav");
	ResourceManager::LoadSound("Res/Sound/SE/Confirm.wav");
	ResourceManager::LoadSound("Res/Sound/SE/Escape.wav");
	ResourceManager::LoadSound("Res/Sound/SE/Select.wav");

	SoundPlayInfo info;
	info.loop = true;
	info.volume = 0.5f;

	SoundPlayer::Play(ResourceManager::GetSound("TutorialBGM.wav"), info);

	std::unique_ptr<Camera> camera{ std::make_unique<Camera>(&worldStorage, objectManager.GenerateNewID()) };
	ResourceManager::LoadCubeTexture("Res/Texture/Sky/NaturalDayMeadow_Cubemap.dds");
	CameraComponent* cameraComponent{ camera->GetComponent<CameraComponent>() };
	cameraComponent->SetSkyTextureHandle(ResourceManager::GetCubeTexture("NaturalDayMeadow_Cubemap.dds"));
	cameraComponent->SetClearMode(ClearMode::SKY);

	Vector3 cameraPos{ Vector3::UP * 2000.0f };

	Renderer::SetCamera(
		MatGenerateFunc::InverseTRS(cameraPos, {}, Vector3{ 1.0f,1.0f, 1.0f }),
		cameraComponent->GetNear(), cameraComponent->GetFar());

	Camera* cameraPtr{ camera.get() };

	objectManager.Add(std::move(camera));

	objectManager.Add(std::make_unique<Player>(&worldStorage, objectManager.GenerateNewID(), cameraPtr));

	objectManager.Add(std::make_unique<TutorialBox>(&worldStorage, objectManager.GenerateNewID()));

	// --- Escape作成 ---
	std::unique_ptr<Escape> escape{ std::make_unique<Escape>(&worldStorage, objectManager.GenerateNewID()) };

	Escape* escapePtr{ escape.get() };

	objectManager.Add(std::move(escape));

	std::unique_ptr<ResumeUI> resume{ std::make_unique<ResumeUI>(&worldStorage, objectManager.GenerateNewID()) };
	escapePtr->AddSlecte(resume.get());

	objectManager.Add(std::move(resume));

	std::unique_ptr<QuitUI> quit{ std::make_unique<QuitUI>(&worldStorage, objectManager.GenerateNewID()) };
	escapePtr->AddSlecte(quit.get());

	objectManager.Add(std::move(quit));

	std::unique_ptr<TitleUI> title{ std::make_unique<TitleUI>(&worldStorage, objectManager.GenerateNewID()) };
	escapePtr->AddSlecte(title.get());

	objectManager.Add(std::move(title));

	std::unique_ptr<Check> check{ std::make_unique<Check>(&worldStorage, objectManager.GenerateNewID()) };
	escapePtr->SetCheck(check.get());

	objectManager.Add(std::move(check));
}