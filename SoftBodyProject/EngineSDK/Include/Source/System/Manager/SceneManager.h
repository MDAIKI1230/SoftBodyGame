#pragma once

#include <memory>

#include "FixedString.h"
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
	// シーン変更適用
	void ApplySceneChange();
	// 終了処理
	void Reset();
private:
	// 初期化用関数
	template<FixedString Name>
	std::unique_ptr<SceneBase> CreateInitialScene();
private:
	// 現在の動いているシーン
	std::unique_ptr<SceneBase> currentScene;
};

#include "SceneManager.inl"