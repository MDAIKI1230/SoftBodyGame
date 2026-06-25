#include "BaseConstants.h"
#include "KeyConstants.h"

#include "ServiceLocator.h"

#include "Dxlib\DxlibRenderer.h"
#include "Dxlib\DxlibInput.h"
#include "Dxlib\DxlibSystem.h"

#include "ApplicationManager.h"

ApplicationManager::ApplicationManager()
{
	system = std::make_unique<DxlibSystem>();

	system->ChangeWindowMode(true);
	system->SetGraphMode(Config::WINDOW_SIZE_W, Config::WINDOW_SIZE_H, Config::COLOR_BIT);

	// 生成
	renderer = std::make_unique<DxlibRenderer>();
	input = std::make_unique<DxlibInput>();
	timeManager = std::make_unique<TimeManager>();
	worldStorage = std::make_unique<WorldStorage>();
	systemManager = std::make_unique<SystemManager>();
	sceneManager = std::make_unique<SceneManager>(worldStorage.get(), systemManager.get());
	eventManager = std::make_unique<EventManager>();
	eventSystem = std::make_unique<EventSystem>();

	// サービスロケータに登録
	ServiceLocator::SetRenderer(renderer.get());
	ServiceLocator::SetInput(input.get());
	ServiceLocator::SetTimeManager(timeManager.get());
}

int ApplicationManager::ApplicationMain()
{
	if (system->Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	renderer->SetUseZDepth(true);
	renderer->SetWriteZDepth(true);

	while (system->ProcessMessage() == 0 && input->GetKeyPress(KeyConstants::ESCAPE) == 0)
	{
		input->Update();
		timeManager->Update();

		systemManager->Update(worldStorage.get(), eventManager.get());

		while (timeManager->IsFixedUpdateTime())
		{
			systemManager->FixedUpdate(worldStorage.get(), eventManager.get());
		}
		sceneManager->Update();

		eventManager->Swap();

		eventSystem->Update(eventManager.get(), sceneManager->GetObjectManager());

		renderer->ClearDrawScreen();

		systemManager->Render(worldStorage.get(), eventManager.get());

		renderer->ScreenFlip();

		timeManager->WaitNextFrame();
	}

	system->End();				// ＤＸライブラリ使用の終了処理

	return 0;
}
