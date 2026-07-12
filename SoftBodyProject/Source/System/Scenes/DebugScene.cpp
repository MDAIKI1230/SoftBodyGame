#include "ServiceLocator.h"

#include "RendererComponent.h"

#include "TransformComponent.h"

#include "SphereColliderComponent.h"

#include "RigidBodyComponent.h"

#include "PointConstraintComponent.h"
#include "DistanceConstraintComponent.h"

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
	Camera camera{ Vector3{0,-175,-500},Vector3{0,-175,0} };
	ServiceLocator::GetRenderer()->SetCamera(camera);

	//objectManager->Add(std::make_unique<DebugSphere>(worldStorage.get(), objectManager->GetHandle()));

	//std::unique_ptr<DebugSphere> sphere{ std::make_unique<DebugSphere>(worldStorage.get(), objectManager->GetHandle()) };
	//// sphere->AddComponent<PointConstraintComponent>();
	//objectManager->Add(std::move(sphere));

	std::unique_ptr<DebugBox> debugBox01{ std::make_unique<DebugBox>(worldStorage.get(), objectManager->GetHandle(), 500.0f, 50.0f, 500.0f)};
	debugBox01->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,-200,0 });
	objectManager->Add(std::move(debugBox01));

	std::unique_ptr<DebugBox> debugBox02{ std::make_unique<DebugBox>(worldStorage.get(), objectManager->GetHandle(), 30.0f) };
	debugBox02->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,50,0 });
	debugBox02->GetComponent<TransformComponent>()->Rotate(Quaternion::AngleAxis((3.141592f / 4.0f), Vector3{ 0,1,1 }));
	debugBox02->AddComponent<RigidBodyComponent>()->SetIsGravity(true);
	DistanceConstraintComponent* pointConstraint{ debugBox02->AddComponent<DistanceConstraintComponent>(Vector3{ 15.0f,16.0f,15.0f },100.0f) };
	objectManager->Add(std::move(debugBox02));

	std::unique_ptr<DebugBox> debugBox03{ std::make_unique<DebugBox>(worldStorage.get(), objectManager->GetHandle(), 30.0f) };
	debugBox03->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,100,0 });
	debugBox03->AddComponent<RigidBodyComponent>()->SetIsGravity(true);
	pointConstraint->AddEndPoint(debugBox03->GetHandle(), Vector3{ 15.0f,15.0f,15.0f });
	objectManager->Add(std::move(debugBox03));

	//std::unique_ptr<DebugBox> debugBox04{ std::make_unique<DebugBox>(worldStorage.get(), objectManager->GetHandle(), 30.0f) };
	//debugBox04->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,100,0 });
	//debugBox04->AddComponent<RigidBodyComponent>()->SetIsGravity(true);
	//pointConstraint->AddEndPoint(debugBox04->GetHandle(), Vector3{ 15.0f,15.0f,15.0f });
	//objectManager->Add(std::move(debugBox04));

	// LoadFile("Res/Data/DebugSceneData.json");

	RendererComponent renderer{ ServiceLocator::GetRenderer()->LoadModel(std::string{"Res/Model/M_001_player_073_01.mv1"}) };
	EntityID id{ objectManager->GetHandle() };
	worldStorage->GetStorage<RendererComponent>()->Add(id, renderer);

	TransformComponent trans{};
	trans.SetPosition(Vector3{ 0,0,0 });

	worldStorage->GetStorage<TransformComponent>()->Add(id,trans);

	state = SceneState::UPDATE;
}
// 終了
void DebugScene::Terminate()
{

}
