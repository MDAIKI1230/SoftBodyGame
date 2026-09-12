#include <algorithm>

#include "TimeManager.h"
#include "ResourceManager.h"

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
#include "HingeConstraintComponentStorage.h"
#include "AngleLimitPointConstraintComponentStorage.h"
#include "AngleLimitHingeConstraintComponentStorage.h"

// キャラクターコントローラー
#include "CharacterControllerComponentStorage.h"

// カメラ
#include "CameraRigSystem.h"
#include "CameraBindSystem.h"
#include "CameraRigComponentStorage.h"

// API
#include "PhysicsComponentAPI.h"
#include "PhysicsAPI.h"

#include "AnimationComponentAPI.h"

#include "SceneDataLoader.h"
#include "ObjectFactory.h"
#include "ComponentFactory.h"

#include "SceneBase.h"

SceneBase::SceneBase()
{
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
	AddStorage<HingeConstraintComponent>(std::make_unique<HingeConstraintComponentStorage>());
	AddStorage<AngleLimitPointConstraintComponent>(std::make_unique<AngleLimitPointConstraintComponentStorage>());
	AddStorage<AngleLimitHingeConstraintComponent>(std::make_unique<AngleLimitHingeConstraintComponentStorage>());

	// キャラクターコントローラー
	AddStorage<CharacterControllerComponent>(std::make_unique<CharacterControllerComponentStorage>());

	// カメラ
	AddSystem(std::make_unique<CameraRigSystem>());
	AddSystem(std::make_unique<CameraBindSystem>());
	AddStorage<CameraRigComponent>(std::make_unique<CameraRigComponentStorage>());

	PhysicsComponentAPI::BindWorld(physicsWorld, worldStorage);
	PhysicsAPI::BindWorld(physicsWorld);

	AnimationComponentAPI::BindWorld(&animationWorld);
}

void SceneBase::Execute()
{
	switch (state)
	{
	case SceneState::INITIALIZE:
		// 初期化タスクの生成
		systemManager.Initialize();
		Initialize();
		state = SceneState::UPDATE;
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
	objectManager.Update();
	// システムマネージャー更新
	systemManager.Update(&worldStorage, &eventManager);
	// アニメーションの更新
	animationWorld.Update(&worldStorage);

	eventManager.Swap();

	eventSystem.Update(&eventManager, &objectManager);

	// 物理更新
	while (TimeManager::IsFixedUpdateTime())
	{
		// アニメーションの物理更新前更新
		animationWorld.PrePhysicsFixedUpdate();

		objectManager.FixedUpdate();

		systemManager.FixedUpdate(&worldStorage, &eventManager);

		physicsWorld.FixedUpdate(&worldStorage, &eventManager);

		// アニメーションの物理更新後更新
		animationWorld.PostPhysicsFixedUpdate();
	}

	// 物理更新終わり描画までのタイミングで更新
	systemManager.LateUpdate(&worldStorage, &eventManager);

	// アニメーションの物理更新後更新
	animationWorld.PreRenderUpdate(&worldStorage);
}

void SceneBase::Render()
{
	systemManager.Render(&worldStorage, &eventManager);
#ifdef _DEBUG
	physicsWorld.DebugRender();
#endif // _DEBUG
}

// 終了
void SceneBase::End()
{
	state = SceneState::TERMINATE;
}

// 切り替えていいよフラグ
bool SceneBase::CompleteEnding()
{
	return isCompleteEnding;
}

void SceneBase::LoadFile(std::string _filePath)
{
	SceneFileData fileData;
	SceneDataLoader::LoadJson(_filePath, fileData);

	for (auto& objData : fileData.objectDatas)
	{
		// 対応オブジェクトを作成
		std::unique_ptr<ObjectBase> obj{ std::move(ObjectFactory::CreateFuncs[objData.type](&worldStorage, objectManager.GenerateNewID())) };

		for (auto& componentData : objData.components)
		{
			ComponentFactory::CreateFuncs[componentData->GetName()](obj.get(), componentData.get());
		}

		objectManager.Add(std::move(obj));
	}
}

void SceneBase::Terminate()
{
	ResourceManager::UnLoadAll();
}

// 仮想デストラクタ
SceneBase::~SceneBase()
{
	ResourceManager::UnLoadAll();
}
