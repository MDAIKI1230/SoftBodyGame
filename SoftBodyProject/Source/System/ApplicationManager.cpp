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
	sceneManager = std::make_unique<SceneManager>();
	timeManager = std::make_unique<TimeManager>();

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

	while (system->ProcessMessage() == 0 && input->GetKeyPress(KeyConstants::ESCAPE) == 0)
	{
		input->Update();
		timeManager->Update();
		sceneManager->Update();

		renderer->ClearDrawScreen();

		sceneManager->Draw();

		renderer->ScreenFlip();

		timeManager->WaitNextFrame();
	}

	system->End();				// ＤＸライブラリ使用の終了処理

	return 0;
}