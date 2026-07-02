#include "ServiceLocator.h"

#include "RendererComponent.h"

#include "TransformComponent.h"

#include "SphereColliderComponent.h"

#include "RigidBodyComponent.h"

#include "DebugSphere.h"
#include "DebugBox.h"

#include "DebugScene.h"

// コンストラクタ
DebugScene::DebugScene()
{
	
}
// 初期化
void DebugScene::Initialize()
{
	Camera camera{ Vector3{0,-200,-500},Vector3{0,0,0} };
	ServiceLocator::GetRenderer()->SetCamera(camera);

	objectManager->Add(std::make_unique<DebugSphere>(worldStorage.get(), 0));

	std::unique_ptr<DebugBox> debugBox01{ std::make_unique<DebugBox>(worldStorage.get(), 1, 300.0f, 50.0f, 300.0f) };
	debugBox01->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,-200,0 });
	objectManager->Add(std::move(debugBox01));

	std::unique_ptr<DebugBox> debugBox02{ std::make_unique<DebugBox>(worldStorage.get(), 2, 30.0f) };
	debugBox02->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,50,0 });
	debugBox02->AddComponent<RigidBodyComponent>()->SetIsGravity(true);
	objectManager->Add(std::move(debugBox02));

	//std::unique_ptr<DebugBox> debugBox03{ std::make_unique<DebugBox>(worldStorage.get(), 3, 30.0f) };
	//debugBox03->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,100,0 });
	//debugBox03->AddComponent<RigidBodyComponent>()->SetIsGravity(true);
	//objectManager->Add(std::move(debugBox03));

	/*RendererComponent renderer{ ServiceLocator::GetRenderer()->LoadModel(std::string{"Res/Model/M_001_player_073_01.mv1"}) };
	
	GetStorage<RendererComponent>()->Add(0,renderer);

	TransformComponent trans{};
	trans.SetPosition(Vector3{ 0,0,0 });

;*/

	state = SceneState::UPDATE;
}
// 終了
void DebugScene::Terminate()
{

}
