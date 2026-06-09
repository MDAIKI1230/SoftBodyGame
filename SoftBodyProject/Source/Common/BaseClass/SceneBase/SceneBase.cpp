#include <algorithm>

#include "SceneBase.h"

#include "ModelRenderingSystem.h"
#include "RendererComponentStorage.h"

#include "TransformComponentStorage.h"

SceneBase::SceneBase()
{
	// レンダリングシステム追加
	AddSystem(std::make_unique<ModelRenderingSystem>());
	// レンダラーストレージ追加
	AddStorage<RendererComponent>(std::make_unique<RendererComponentStorage>());
	// Transformも同様
	AddStorage<TransformComponent>(std::make_unique<TransformComponentStorage>());
}

void SceneBase::Execute()
{
	switch (state)
	{
	case SceneState::INITIALIZE:
		// 初期化タスクの生成
		Initialize();
		break;
	case SceneState::LOADING:
		// wait処理
		break;
	case SceneState::FADEIN:
		// ロード画面からシーンに移行。
		FadeIn();
		break;
	case SceneState::UPDATE:
		// 更新(物理更新も含む)
		Update();
		break;
	case SceneState::TERMINATE:
		// 終了
		Terminate();
		break;
	case SceneState::FADEOUT:
		// ロードシーンに移行
		FadeOut();
		break;
	default:
		break;
	}
}

/// <summary>
/// システムの追加(moveされる)
/// </summary>
/// <param name="system">入れたいシステム</param>
void SceneBase::AddSystem(std::unique_ptr<UpdateSystem> _system)
{
	// 入れる位置を探す
	auto it = std::lower_bound(
		updateSystems.begin(),
		updateSystems.end(),
		_system,
		[](const std::unique_ptr<UpdateSystem>& a, const std::unique_ptr<UpdateSystem>& b)
		{
			return a->GetPriority() > b->GetPriority();
		});

	// その位置に挿入
	updateSystems.insert(it, std::move(_system));
}

/// <summary>
/// システムの追加(moveされる)
/// </summary>
/// <param name="system">入れたいシステム</param>
void SceneBase::AddSystem(std::unique_ptr<RenderingSystem> _system)
{
	// 入れる位置を探す
	auto it = std::lower_bound(
		renderingSystems.begin(),
		renderingSystems.end(),
		_system,
		[](const std::unique_ptr<RenderingSystem>& a, const std::unique_ptr<RenderingSystem>& b)
		{
			return a->GetPriority() > b->GetPriority();
		});

	// その位置に挿入
	renderingSystems.insert(it, std::move(_system));
}

void SceneBase::FadeIn()
{
	
}

void SceneBase::FadeOut()
{

}

void SceneBase::Update()
{
	// 更新
	for (int i{ 0 }; i < updateSystems.size(); i++)
	{
		updateSystems[i]->Update(this);
	}
}

// 描画
void SceneBase::Draw()
{
	// 更新
	for (int i{ 0 }; i < renderingSystems.size(); i++)
	{
		renderingSystems[i]->Draw(this);
	}
}