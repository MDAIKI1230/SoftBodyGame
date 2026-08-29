#include "BaseConstants.h"
#include "KeyConstants.h"

#include "ServiceLocator.h"

#include "ApplicationManager.h"

ApplicationManager::ApplicationManager(BackEnd&& _backEnd):
	renderer{ std::move(_backEnd.renderer) },
	input{ std::move(_backEnd.input) },
	system{ std::move(_backEnd.system) },
	gpuConnecter{ std::move(_backEnd.gpuConnecter) }

{
	system->ChangeWindowMode(true);
	system->SetGraphMode(Config::WINDOW_SIZE_W, Config::WINDOW_SIZE_H, Config::COLOR_BIT);

	timeManager = std::make_unique<TimeManager>();
	resourceManager = std::make_unique<ResourceManager>();

	// サービスロケータに登録
	ServiceLocator::SetRenderer(renderer.get());
	ServiceLocator::SetInput(input.get());
	ServiceLocator::SetGPUConnecter(gpuConnecter.get());
	ServiceLocator::SetTimeManager(timeManager.get());
	ServiceLocator::SetResourceManager(resourceManager.get());

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

	input->Initialize();

	while (system->ProcessMessage() == 0)
	{
		input->Update();
		timeManager->Update();
		sceneManager->Update();

		renderer->ClearDrawScreen();

		sceneManager->Render();

		renderer->ScreenFlip();

		timeManager->WaitNextFrame();
	}

	system->End();				// ＤＸライブラリ使用の終了処理

	return 0;
}
