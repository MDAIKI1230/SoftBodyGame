#pragma once

#include <memory>
#include <filesystem>
#include <unordered_map>

#include "SceneBase.h"

class SceneManager
{
public:
	// コンストラクタ
	SceneManager();
	// 更新
	void Update();
	// 描画
	void Render();

	// シーン変更リクエスト
	void RequestSceneChange(std::filesystem::path _path);

	// シーン変更適用
	void ApplySceneChange();
private:
	// 現在の動いているシーン
	std::unique_ptr<SceneBase> currentScene;
	// 次に動かせるシーン
	// リクエストが来るとここにいったん入れる
	std::unique_ptr<SceneBase> nextScene;

	// すべてのシーンのパスを保存しておく奴
	std::unordered_map<std::filesystem::path, std::filesystem::path> paths;
};
