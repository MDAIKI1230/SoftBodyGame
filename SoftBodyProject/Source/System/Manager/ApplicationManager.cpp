#include "BaseConstants.h"

#include "ApplicationRequest.h"

#include "ServiceLocator.h"
#include "TimeManager.h"
#include "InputSystem.h"

#include "ApplicationManager.h"

ApplicationManager::ApplicationManager(BackEnd&& _backEnd):
	renderer{ std::move(_backEnd.renderer) },
	system{ std::move(_backEnd.system) },
	gpuConnecter{ std::move(_backEnd.gpuConnecter) },
	sound{ std::move(_backEnd.sound) }
{
	system->ChangeWindowMode(true);
	system->SetGraphMode(Config::WINDOW_SIZE_W, Config::WINDOW_SIZE_H, Config::COLOR_BIT);

	// サービスロケータに登録
	ServiceLocator::SetRenderer(renderer.get());
	ServiceLocator::SetGPUConnecter(gpuConnecter.get());
	ServiceLocator::SetSound(sound.get());

	sceneManager = std::make_unique<SceneManager>();
}

int ApplicationManager::ApplicationMain()
{
	if (system->Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	renderer->SetUseZDepth(true);
	renderer->SetWriteZDepth(true);

	InputSystem::Initialize();

	while (system->ProcessMessage() == 0 && !ApplicationRequest::IsExitRequested())
	{
		InputSystem::Update();
		TimeManager::Update();
		sceneManager->Update();

		renderer->ClearDrawScreen();

		sceneManager->Render();

		renderer->ScreenFlip();

		sceneManager->ApplySceneChange();

		TimeManager::WaitNextFrame();
	}

	system->End();				// ＤＸライブラリ使用の終了処理

	return 0;
}
