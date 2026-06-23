#include <algorithm>

#include "ServiceLocator.h"

// 描画系
#include "ModelRenderingSystem.h"
#include "RendererComponentStorage.h"
#include "DebugRenderingSystem.h"

// Transform
#include "TransformComponentStorage.h"


// 物理系
#include "SphereColliderComponentStorage.h"
#include "RigidBodyComponentStorage.h"

#include "RigidBodySystem.h"
#include "CollisionSystem.h"
#include "PhysicsCommitSystem.h"

// API
#include "PhysicsAPI.h"

#include "SceneBase.h"

SceneBase::SceneBase()
{
	// レンダリングシステム追加
	AddSystem(std::make_unique<ModelRenderingSystem>());
	// レンダラーストレージ追加
	AddStorage<RendererComponent>(std::make_unique<RendererComponentStorage>());
	// Transformも同様
	AddStorage<TransformComponent>(std::make_unique<TransformComponentStorage>());
	AddSystem(std::make_unique<CollisionSystem>());
	AddStorage<SphereColliderComponent>(std::make_unique<SphereColliderComponentStorage>());
	AddSystem(std::make_unique<RigidBodySystem>());
	AddStorage<RigidBodyComponent>(std::make_unique<RigidBodyComponentStorage>());
	AddSystem(std::make_unique<PhysicsCommitSystem>());
	// オブジェクトマネージャー
	objectManager = std::make_unique<ObjectManager>();

	PhysicsAPI::SetWorld(this);

#ifdef _DEBUG
	AddSystem(std::make_unique<DebugRenderingSystem>());
#endif // _DEBUG
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
void SceneBase::AddSystem(std::unique_ptr<FixedUpdateSystem> _system)
{
	// 入れる位置を探す
	auto it = std::lower_bound(
		fixedUpdateSystems.begin(),
		fixedUpdateSystems.end(),
		_system,
		[](const std::unique_ptr<FixedUpdateSystem>& a, const std::unique_ptr<FixedUpdateSystem>& b)
		{
			return a->GetPriority() > b->GetPriority();
		});

	// その位置に挿入
	fixedUpdateSystems.insert(it, std::move(_system));
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

// オブジェクトマネージャー取得
ObjectManager* SceneBase::GetObjectManager()
{
	return objectManager.get();
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
	// オブジェクトマネージャー更新
	objectManager->Update();

	// 物理更新
	while (ServiceLocator::GetTimeManager()->IsFixedUpdateTime())
	{
		for (int i{ 0 }; i < fixedUpdateSystems.size(); i++)
		{
			fixedUpdateSystems[i]->FixedUpdate(this);
		}

		objectManager->FixedUpdate();
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
