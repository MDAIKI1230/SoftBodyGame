#include "ServiceLocator.h"

#include "RendererComponent.h"

#include "TransformComponent.h"

#include "SphereColliderComponent.h"

#include "RigidBodyComponent.h"

#include "PointConstraintComponent.h"
#include "DistanceConstraintComponent.h"

#include "DebugSphere.h"
#include "DebugBox.h"
#include "EmptyObject.h"

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

	// 距離拘束デバッグ
	std::unique_ptr<EmptyObject> emptyObject{ std::make_unique<EmptyObject>(worldStorage.get(), objectManager->GetHandle()) };
	emptyObject->GetComponent<TransformComponent>()->SetPosition(Vector3{ 200.0f,0.0f,0.0f });
	DistanceConstraintComponent* distanceConstraint{ emptyObject->AddComponent<DistanceConstraintComponent>(Vector3{ 15.0f,16.0f,15.0f },100.0f) };
	objectManager->Add(std::move(emptyObject));


	std::unique_ptr<DebugBox> debugBox03{ std::make_unique<DebugBox>(worldStorage.get(), objectManager->GetHandle(), 30.0f) };
	debugBox03->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,100,0 });
	debugBox03->AddComponent<RigidBodyComponent>()->SetIsGravity(true);
	distanceConstraint->AddEndPoint(debugBox03->GetHandle(), Vector3{ 15.0f,15.0f,15.0f });
	objectManager->Add(std::move(debugBox03));

	// 点拘束デバッグ
	//std::unique_ptr<DebugBox> debugBox02{ std::make_unique<DebugBox>(worldStorage.get(), objectManager->GetHandle(), 30.0f) };
	//debugBox02->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,50,0 });
	//debugBox02->GetComponent<TransformComponent>()->Rotate(Quaternion::AngleAxis((3.141592f / 4.0f), Vector3{ 0,1,1 }));
	//debugBox02->AddComponent<RigidBodyComponent>()->SetIsGravity(true);
	//PointConstraintComponent* pointConstraint{ debugBox02->AddComponent<PointConstraintComponent>(Vector3{ 15.0f,15.0f,15.0f }) };
	//objectManager->Add(std::move(debugBox02));

	//std::unique_ptr<DebugBox> debugBox04{ std::make_unique<DebugBox>(worldStorage.get(), objectManager->GetHandle(), 30.0f) };
	//debugBox04->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,100,0 });
	//debugBox04->AddComponent<RigidBodyComponent>()->SetIsGravity(true);
	//pointConstraint->AddEndPoint(debugBox04->GetHandle(), Vector3{ 15.0f,15.0f,15.0f });
	//objectManager->Add(std::move(debugBox04));

	// 距離拘束によるロープの実装
	float distance{ 15.0f };

	std::unique_ptr<EmptyObject> empty1{ std::make_unique<EmptyObject>(worldStorage.get(), objectManager->GetHandle()) };
	distanceConstraint = empty1->AddComponent<DistanceConstraintComponent>(distance);
	objectManager->Add(std::move(empty1));

	for (int i{ 0 }; i < 10; i++)
	{
		std::unique_ptr<DebugSphere> point{ std::make_unique<DebugSphere>(worldStorage.get(), objectManager->GetHandle()) };
		point->GetComponent<SphereColliderComponent>()->SetRadius(2.0f);
		point->GetComponent<TransformComponent>()->SetPosition(Vector3{ distance * i,0.0f,0.0f });
		distanceConstraint->AddEndPoint(point->GetHandle(), Vector3::ZERO);
		distanceConstraint = point->AddComponent<DistanceConstraintComponent>(distance);
		objectManager->Add(std::move(point));
	}

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
