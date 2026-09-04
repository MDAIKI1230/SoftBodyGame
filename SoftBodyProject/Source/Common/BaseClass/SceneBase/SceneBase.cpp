#include <algorithm>

#include "ServiceLocator.h"

// 描画系
#include "ModelRenderingSystem.h"
#include "RendererComponentStorage.h"
#include "CameraComponentStorage.h"
#include "SkyRenderingSystem.h"

// Transform
#include "TransformComponentStorage.h"


// 物理系
// コライダー
#include "SphereColliderComponentStorage.h"
#include "BoxColliderComponentStorage.h"
#include "CapsuleColliderComponentStorage.h"

// ボディ
#include "RigidBodyComponentStorage.h"
#include "RopeComponentStorage.h"
#include "ClothComponentStorage.h"
#include "SoftBodyComponentStorage.h"

// 拘束
#include "PointConstraintComponentStorage.h"
#include "DistanceConstraintComponentStorage.h"

// キャラクターコントローラー
#include "CharacterControllerComponentStorage.h"

// カメラ
#include "CameraRigSystem.h"
#include "CameraBindSystem.h"
#include "CameraRigComponentStorage.h"

// API
#include "PhysicsComponentAPI.h"
#include "PhysicsAPI.h"

#include "SceneDataLoader.h"
#include "ObjectFactory.h"
#include "ComponentFactory.h"

#include "SceneBase.h"

SceneBase::SceneBase()
{
	worldStorage = std::make_unique<WorldStorage>();
	systemManager = std::make_unique<SystemManager>();
	eventManager = std::make_unique<EventManager>();
	eventSystem = std::make_unique<EventSystem>();
	physicsWorld = std::make_unique<PhysicsWorld>();

	// レンダリングシステム追加
	AddSystem(std::make_unique<ModelRenderingSystem>());
	AddSystem(std::make_unique<SkyRenderingSystem>());
	// レンダラーストレージ追加
	AddStorage<RendererComponent>(std::make_unique<RendererComponentStorage>());
	// カメラコンポーネントストレージ追加
	AddStorage<CameraComponent>(std::make_unique<CameraComponentStorage>());
	// Transformも同様
	AddStorage<TransformComponent>(std::make_unique<TransformComponentStorage>());
	// 物理関係
	// コライダー
	AddStorage<SphereColliderComponent>(std::make_unique<SphereColliderComponentStorage>());
	AddStorage<BoxColliderComponent>(std::make_unique<BoxColliderComponentStorage>());
	AddStorage<CapsuleColliderComponent>(std::make_unique<CapsuleColliderComponentStorage>());

	// ボディ
	AddStorage<RigidBodyComponent>(std::make_unique<RigidBodyComponentStorage>());
	AddStorage<RopeComponent>(std::make_unique<RopeComponentStorage>());
	AddStorage<ClothComponent>(std::make_unique<ClothComponentStorage>());
	AddStorage<SoftBodyComponent>(std::make_unique<SoftBodyComponentStorage>());

	// 拘束
	AddStorage<PointConstraintComponent>(std::make_unique<PointConstraintComponentStorage>());
	AddStorage<DistanceConstraintComponent>(std::make_unique<DistanceConstraintComponentStorage>());

	// キャラクターコントローラー
	AddStorage<CharacterControllerComponent>(std::make_unique<CharacterControllerComponentStorage>());

	// カメラ
	AddSystem(std::make_unique<CameraRigSystem>());
	AddSystem(std::make_unique<CameraBindSystem>());
	AddStorage<CameraRigComponent>(std::make_unique<CameraRigComponentStorage>());

	// オブジェクトマネージャー
	objectManager = std::make_unique<ObjectManager>();

	PhysicsComponentAPI::BindWorld(*physicsWorld.get(), *worldStorage.get());
	PhysicsAPI::BindWorld(*physicsWorld.get());
}

void SceneBase::Execute()
{
	switch (state)
	{
	case SceneState::INITIALIZE:
		// 初期化タスクの生成
		systemManager->Initialize();
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
		PhysicsComponentAPI::UnbindWorld();
		PhysicsAPI::UnbindWorld();
		break;
	case SceneState::FADEOUT:
		// ロードシーンに移行
		FadeOut();
		break;
	default:
		break;
	}
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

		physicsWorld->FixedUpdate(worldStorage.get(), eventManager.get());
	}

	// 物理更新終わり描画までのタイミングで更新
	systemManager->LateUpdate(worldStorage.get(), eventManager.get());
}

void SceneBase::Render()
{
	systemManager->Render(worldStorage.get(), eventManager.get());
#ifdef _DEBUG
	physicsWorld->DebugRender();
#endif // _DEBUG
}

void SceneBase::LoadFile(std::string _filePath)
{
	SceneFileData fileData;
	SceneDataLoader::LoadJson(_filePath, fileData);

	for (auto& objData : fileData.objectDatas)
	{
		// 対応オブジェクトを作成
		std::unique_ptr<ObjectBase> obj{ std::move(ObjectFactory::CreateFuncs[objData.type](worldStorage.get(), objectManager->GenerateNewID())) };

		for (auto& componentData : objData.components)
		{
			ComponentFactory::CreateFuncs[componentData->GetName()](obj.get(), componentData.get());
		}

		objectManager->Add(std::move(obj));
	}
}
