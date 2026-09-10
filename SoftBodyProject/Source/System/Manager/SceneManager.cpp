# include <Windows.h>
# include <string>
# include <vector>

#include "Scenes/DebugScene.h"

#include "SceneManager.h"

SceneManager::SceneManager()
{
	currentScene = std::make_unique<DebugScene>();

	// "Res/Data/Scene"内のフォルダを取得
	std::vector<std::filesystem::path> filePaths;

	for (const std::filesystem::directory_entry& de : std::filesystem::recursive_directory_iterator("Res/Data/Scene"))
	{
		filePaths.emplace_back(de.path().string());
	}

	for (auto& filePath : filePaths)
	{
		// 拡張子を除いたファイル名を取得する
		paths[filePath.filename().stem()] = filePath;
	}
}

void SceneManager::Update()
{
	currentScene->Execute();
}

void SceneManager::Render()
{
	currentScene->Render();
}

// シーン変更リクエスト
void SceneManager::RequestSceneChange(std::filesystem::path _path)
{
	// nextScene = paths[_path];
}

// シーン変更適用
void SceneManager::ApplySceneChange()
{
	if (nextScene.get() != nullptr)
	{
		if (currentScene->CompleteEnding())
		{
			currentScene = std::move(nextScene);
		}
	}
}
