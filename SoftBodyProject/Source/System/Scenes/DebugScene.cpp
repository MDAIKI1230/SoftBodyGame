#include "ServiceLocator.h"

#include "RendererComponent.h"

#include "TransformComponent.h"

#include "ColliderComponent.h"
#include "SphereColliderComponent.h"
#include "SphereColliderComponentStorage.h"
#include "CollisionSystem.h"

#include "DebugSphere.h"

#include "DebugScene.h"

// コンストラクタ
DebugScene::DebugScene()
{
	AddSystem(std::make_unique<CollisionSystem>());
	AddStorage<SphereColliderComponent>(std::make_unique<SphereColliderComponentStorage>());
}
// 初期化
void DebugScene::Initialize()
{
	Camera camera{ Vector3{0,0,-500},Vector3{0,0,0} };
	ServiceLocator::GetRenderer()->SetCamera(camera);

	objectManager->Add(std::make_unique<DebugSphere>(this, 0));

	/*RendererComponent renderer{ ServiceLocator::GetRenderer()->LoadModel(std::string{"Res/Model/M_001_player_073_01.mv1"}) };
	
	GetStorage<RendererComponent>()->Add(0,renderer);

	TransformComponent trans{};
	trans.SetPosition(Vector3{ 0,0,0 });

	GetStorage<TransformComponent>()->Add(0, trans);

	GetStorage<RendererComponent>()->Add(1, renderer);

	trans.SetPosition(Vector3{ 0,0,-300 });

	GetStorage<TransformComponent>()->Add(1, trans);

	SphereColliderComponent scc{ 30.0f };
	GetStorage<SphereColliderComponent>()->Add(1, scc);

	TransformComponent trans01{};

	trans01.SetPosition(Vector3{ 0,0,-300 });

	GetStorage<TransformComponent>()->Add(1, trans01);*/

	state = SceneState::UPDATE;
}
// 終了
void DebugScene::Terminate()
{

}