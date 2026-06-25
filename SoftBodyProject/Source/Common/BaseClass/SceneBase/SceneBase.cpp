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
	worldStorage = std::make_unique<WorldStorage>();
	systemManager = std::make_unique<SystemManager>();
	eventManager = std::make_unique<EventManager>();
	eventSystem = std::make_unique<EventSystem>();

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

	PhysicsAPI::SetWorld(worldStorage.get());

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
void SceneBase::AddSystem(std::unique_ptr<UpdateSystem>&& _system)
{
	systemManager->AddSystem(std::move(_system));
}

/// <summary>
/// システムの追加(moveされる)
/// </summary>
/// <param name="system">入れたいシステム</param>
void SceneBase::AddSystem(std::unique_ptr<FixedUpdateSystem>&& _system)
{
	systemManager->AddSystem(std::move(_system));
}

/// <summary>
/// システムの追加(moveされる)
/// </summary>
/// <param name="system">入れたいシステム</param>
void SceneBase::AddSystem(std::unique_ptr<RenderingSystem>&& _system)
{
	systemManager->AddSystem(std::move(_system));
}

void SceneBase::FadeIn()
{
	
}

void SceneBase::FadeOut()
{

}

void SceneBase::Update()
{
	// オブジェクトマネージャー更新
	objectManager->Update();
	// システムマネージャー更新
	systemManager->Update(worldStorage.get(), eventManager.get());

	eventManager->Swap();

	eventSystem->Update(eventManager.get(), objectManager.get());

	// 物理更新
	while (ServiceLocator::GetTimeManager()->IsFixedUpdateTime())
	{
		objectManager->FixedUpdate();

		systemManager->FixedUpdate(worldStorage.get(), eventManager.get());
	}
}

void SceneBase::Render()
{
	systemManager->Render(worldStorage.get(), eventManager.get());
}
