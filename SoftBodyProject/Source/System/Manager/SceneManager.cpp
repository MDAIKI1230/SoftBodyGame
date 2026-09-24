# include <string>

#include "Scenes/DebugScene.h"
#include "Scenes/TitleScene.h"

#include "ApplicationRequest.h"

#include "SceneManager.h"

SceneManager::SceneManager()
{
	currentScene = std::make_unique<TitleScene>();

	// "Res/Data/Scene"内のフォルダを取得
	//std::vector<std::filesystem::path> filePaths;

	//for (const std::filesystem::directory_entry& de : std::filesystem::recursive_directory_iterator("Res/Data/Scene"))
	//{
	//	filePaths.emplace_back(de.path().string());
	//}

	//for (auto& filePath : filePaths)
	//{
	//	// 拡張子を除いたファイル名を取得する
	//	paths[filePath.filename().stem()] = filePath;
	//}
}

void SceneManager::Update()
{
	currentScene->Execute();
}

void SceneManager::Render()
{
	currentScene->Render();
}

// シーン変更適用
void SceneManager::ApplySceneChange()
{
	if (ApplicationRequest::GetSceneRequest() != SceneRequestType::NONE)
	{
		currentScene->End();
	}

	if (currentScene->CompleteEnding())
	{
		switch (ApplicationRequest::GetSceneRequest())
		{
		case SceneRequestType::TITLE:
			currentScene = std::make_unique<TitleScene>();
			ApplicationRequest::SceneRequest(SceneRequestType::NONE);
			break;
		case SceneRequestType::GAME:
			currentScene = std::make_unique<DebugScene>();
			ApplicationRequest::SceneRequest(SceneRequestType::NONE);
			break;
		case SceneRequestType::NONE:
		default:
			break;
		}
	}
}