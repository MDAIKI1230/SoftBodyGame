#include <algorithm>

#include "TimeManager.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "SoundPlayer.h"

// 描画系
#include "RendererComponentStorage.h"
#include "CameraComponentStorage.h"
#include "SpriteRendererComponentStorage.h"
#include "ModelRenderingSystem.h"
#include "SkyRenderingSystem.h"
#include "SpriteRenderingSystem.h"


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
#include "LimitedBallJointConstraintComponentStorage.h"

// キャラクターコントローラー
#include "CharacterControllerComponentStorage.h"

// カメラ
#include "CameraRigSystem.h"
#include "CameraBindSystem.h"
#include "CameraRigComponentStorage.h"

// アニメーション関系

#include "AnimationComponentStorage.h"

#include "RagdollComponentStorage.h"
#include "ActiveRagdollComponentStorage.h"

#include "HandIKComponentStorage.h"

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
	// レンダラーストレージ追加
	AddStorage<RendererComponent>(std::make_unique<RendererComponentStorage>());
	// カメラコンポーネントストレージ追加
	AddStorage<CameraComponent>(std::make_unique<CameraComponentStorage>());
	// スプライトコンポーネントストレージ追加
	AddStorage<SpriteRendererComponent>(std::make_unique<SpriteRendererComponentStorage>());
	// レンダリングシステム追加
	AddSystem(std::make_unique<ModelRenderingSystem>());
	AddSystem(std::make_unique<SkyRenderingSystem>());
	AddSystem(std::make_unique<SpriteRenderingSystem>());
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
	AddStorage<LimitedBallJointConstraintComponent>(std::make_unique<LimitedBallJointConstraintComponentStorage>());

	// キャラクターコントローラー
	AddStorage<CharacterControllerComponent>(std::make_unique<CharacterControllerComponentStorage>());

	// カメラ
	AddSystem(std::make_unique<CameraRigSystem>());
	AddSystem(std::make_unique<CameraBindSystem>());
	AddStorage<CameraRigComponent>(std::make_unique<CameraRigComponentStorage>());

	// アニメーション関系

	AddStorage<AnimationComponent>(std::make_unique<AnimationComponentStorage>());

	AddStorage<RagdollComponent>(std::make_unique<RagdollComponentStorage>());
	AddStorage<ActiveRagdollComponent>(std::make_unique<ActiveRagdollComponentStorage>());

	AddStorage<HandIKComponent>(std::make_unique<HandIKComponentStorage>());

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
		fade.StartFadeIn();
		GameManager::StartScene();
		TimeManager::ResetTime();
		state = SceneState::FADEIN;
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
	fade.Update();
	if (fade.IsFadeInFinished())
	{
		state = SceneState::UPDATE;
	}
}

void SceneBase::FadeOut()
{
	fade.Update();
	if (fade.IsFadeOutFinished())
	{
		state = SceneState::TERMINATE;
	}
}

void SceneBase::Update()
{
	// オブジェクトマネージャー更新
	objectManager.Update();
	// システムマネージャー更新
	systemManager.Update(&worldStorage, &eventManager);
	// アニメーションの更新
	animationWorld.Update(&worldStorage);
	// サウンドプレイヤーの更新
	SoundPlayer::Update();

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

	fade.Draw();
}

// 終了
void SceneBase::End()
{
	if (state != SceneState::UPDATE)
	{
		return;
	}

	state = SceneState::FADEOUT;
	fade.StartFadeOut();
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
	SoundPlayer::StopAll();
	ResourceManager::UnLoadAll();
	isCompleteEnding = true;
}

// 仮想デストラクタ
SceneBase::~SceneBase()
{
}