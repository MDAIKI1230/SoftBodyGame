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

	objectManager->Add(std::make_unique<DebugSphere>(worldStorage.get(), objectManager->GenerateNewID()));

	//std::unique_ptr<DebugSphere> sphere{ std::make_unique<DebugSphere>(worldStorage.get(), objectManager->GenerateNewID()) };
	//// sphere->AddComponent<PointConstraintComponent>();
	//objectManager->Add(std::move(sphere));

	std::unique_ptr<DebugBox> debugBox01{ std::make_unique<DebugBox>(worldStorage.get(), objectManager->GenerateNewID(), 500.0f, 50.0f, 500.0f)};
	debugBox01->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,-200,0 });
	objectManager->Add(std::move(debugBox01));

	// 距離拘束デバッグ(宙ぶらりんなせいで力が減衰する要素がほぼないので凄い動く)
	/*std::unique_ptr<EmptyObject> emptyObject{ std::make_unique<EmptyObject>(worldStorage.get(), objectManager->GenerateNewID()) };
	emptyObject->GetComponent<TransformComponent>()->SetPosition(Vector3{ 200.0f,0.0f,0.0f });
	DistanceConstraintComponent* distanceConstraint{ emptyObject->AddComponent<DistanceConstraintComponent>(Vector3{ 15.0f,15.0f,15.0f },100.0f) };
	objectManager->Add(std::move(emptyObject));


	std::unique_ptr<DebugBox> debugBox03{ std::make_unique<DebugBox>(worldStorage.get(), objectManager->GenerateNewID(), 30.0f) };
	debugBox03->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,100,0 });
	debugBox03->AddComponent<RigidBodyComponent>()->SetIsGravity(true);
	distanceConstraint->AddEndPoint(debugBox03->GetHandle(), Vector3{ 15.0f,15.0f,15.0f });
	objectManager->Add(std::move(debugBox03));*/

	// 点拘束デバッグ
	std::unique_ptr<DebugBox> debugBox02{ std::make_unique<DebugBox>(worldStorage.get(), objectManager->GenerateNewID(), 30.0f) };
	debugBox02->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,50,0 });
	debugBox02->GetComponent<TransformComponent>()->Rotate(Quaternion::AngleAxis((3.141592f / 4.0f), Vector3{ 0,1,1 }));
	debugBox02->AddComponent<RigidBodyComponent>()->SetIsGravity(true);
	PointConstraintComponent* pointConstraint{ debugBox02->AddComponent<PointConstraintComponent>(Vector3{ 15.0f,15.0f,15.0f }) };
	objectManager->Add(std::move(debugBox02));

	std::unique_ptr<DebugBox> debugBox04{ std::make_unique<DebugBox>(worldStorage.get(), objectManager->GenerateNewID(), 30.0f) };
	debugBox04->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,100,0 });
	debugBox04->AddComponent<RigidBodyComponent>()->SetIsGravity(true);
	pointConstraint->AddEndPoint(debugBox04->GetHandle(), Vector3{ 15.0f,15.0f,15.0f });
	objectManager->Add(std::move(debugBox04));

	// 距離拘束によるロープの実装
	/*
			思ったよりええ感じ。
			当り判定は質点の球のみだからコリジョン抜けしまくる
	*/
	/*float distanceRope{ 15.0f };

	Vector3 ropePosition{ -200.0f,0.0f,0.0f };

	std::unique_ptr<EmptyObject> empty1{ std::make_unique<EmptyObject>(worldStorage.get(), objectManager->GenerateNewID()) };
	empty1->GetComponent<TransformComponent>()->SetPosition(ropePosition);
	DistanceConstraintComponent* distanceConstraintRope = empty1->AddComponent<DistanceConstraintComponent>(distanceRope);
	objectManager->Add(std::move(empty1));

	for (int i{ 0 }; i < 10; i++)
	{
		std::unique_ptr<DebugSphere> point{ std::make_unique<DebugSphere>(worldStorage.get(), objectManager->GenerateNewID()) };
		point->GetComponent<SphereColliderComponent>()->SetRadius(2.0f);
		point->GetComponent<TransformComponent>()->SetPosition(ropePosition + Vector3{ distanceRope * i,0.0f,0.0f });
		distanceConstraintRope->AddEndPoint(point->GetHandle(), Vector3::ZERO);
		distanceConstraintRope = point->AddComponent<DistanceConstraintComponent>(distanceRope);
		objectManager->Add(std::move(point));
	}*/

	// 布のテスト
	/*
			ただ拘束をつなげただけだと柔らかさがない。
			当り判定はロープと同じ
	*/
	float distanceCloth{ 20.0f };

	std::vector<DistanceConstraintComponent*> distanceConstraints;
	std::vector<std::unique_ptr<ObjectBase>> objects;

	int width{ 8 };
	int height{ 6};
	EntityID handle{ objectManager->GenerateNewID() };
	for (int i{ 0 }; i < height; i++)
	{
		for (int j{ 0 }; j < width; j++)
		{
			if ((i == 0 && j == 0) || (i == 0 && j == (width - 1)))
			{
				std::unique_ptr<EmptyObject> point{ std::make_unique<EmptyObject>(worldStorage.get(), EntityID{handle.GetIndex() + static_cast<uint32_t>(objects.size()),1}) };
				point->GetComponent<TransformComponent>()->SetPosition(Vector3{ distanceCloth * j,0.0f,distanceCloth * i });
				point->AddComponent<DistanceConstraintComponent>(distanceCloth);
				objects.push_back(std::move(point));
			}
			else
			{
				std::unique_ptr<DebugSphere> point{ std::make_unique<DebugSphere>(worldStorage.get(),EntityID{handle.GetIndex() + static_cast<uint32_t>(objects.size()),1}) };
				point->GetComponent<SphereColliderComponent>()->SetRadius(2.0f);
				point->GetComponent<TransformComponent>()->SetPosition(Vector3{ distanceCloth * j,0.0f,distanceCloth * i });
				point->AddComponent<DistanceConstraintComponent>(distanceCloth);
				objects.push_back(std::move(point));
			}
		}
	}

	for (auto& obj : objects)
	{
		distanceConstraints.push_back(obj->GetComponent<DistanceConstraintComponent>());
	}

	for (int i{ 0 }; i < width * height; i++)
	{
		// 横のつながり
		if (i % width != (width - 1))
		{
			distanceConstraints[i]->AddEndPoint(objects[i + 1]->GetHandle(), Vector3::ZERO);
		}
		// 縦のつながり
		if (i / width != (height - 1))
		{
			distanceConstraints[i]->AddEndPoint(objects[i + width]->GetHandle(), Vector3::ZERO);
		}
	}

	for (auto& obj : objects)
	{
		objectManager->Add(std::move(obj));
	}

	/*
			おそらくソフトボディは、成り立たない体積保管がないから。
	*/

	// LoadFile("Res/Data/DebugSceneData.json");
#ifdef USE_RAYLIB
	RendererComponent renderer{ ServiceLocator::GetRenderer()->LoadModel(std::string{"Res/Model/Player.glb"}) };
#else
	RendererComponent renderer{ ServiceLocator::GetRenderer()->LoadModel(std::string{"Res/Model/M_001_player_073_01.mv1"}) };
#endif // PLAY_RAYLIB

	
	EntityID id{ objectManager->GenerateNewID() };
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
