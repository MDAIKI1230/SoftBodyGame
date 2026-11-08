#include "InputSystem.h"
#include "GameManager.h"
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
	GameManager::StartScene();

	InputSystem::LoadAsset("Res/Data/Input/CharacterInput.json");
	ResourceManager::LoadModel("Res/Model/M_001_player_095_01.mv1");
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

	Camera* camera{ Create<Camera>() };
	ResourceManager::LoadCubeTexture("Res/Texture/Sky/NaturalDayMeadow_Cubemap.dds");
	CameraComponent* cameraComponent{ camera->GetComponent<CameraComponent>() };
	cameraComponent->SetSkyTextureHandle(ResourceManager::GetCubeTexture("NaturalDayMeadow_Cubemap.dds"));
	cameraComponent->SetClearMode(ClearMode::SKY);
	cameraComponent->SetIsMain(true);

	Vector3 cameraPos{ Vector3::UP * 2000.0f };

	Renderer::SetCamera(
		MatGenerateFunc::InverseTRS(cameraPos, {}, Vector3{ 1.0f,1.0f, 1.0f }),
		cameraComponent->GetNear(), cameraComponent->GetFar());

	Camera* cameraPtr{ camera };

	Camera::main = cameraPtr;

	Create<Player>();

	Create<TutorialBox>();

	// --- Escape作成 ---
	Escape* escape{ Create<Escape>() };

	Escape* escapePtr{ escape };

	ResumeUI* resume{ Create<ResumeUI>() };
	escapePtr->AddSlecte(resume);

	QuitUI* quit{ Create<QuitUI>() };
	escapePtr->AddSlecte(quit);

	TitleUI* title{ Create<TitleUI>() };
	escapePtr->AddSlecte(title);

	Check* check{ Create<Check>() };
	escapePtr->SetCheck(check);
}