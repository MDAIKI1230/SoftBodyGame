#include "InputSystem.h"
#include "ResourceManager.h"

#include "RendererComponent.h"

#include "TransformComponent.h"

#include "SphereColliderComponent.h"
#include "BoxColliderComponent.h"
#include "CapsuleColliderComponent.h"

#include "RigidBodyComponent.h"

#include "PointConstraintComponent.h"
#include "DistanceConstraintComponent.h"
#include "HingeConstraintComponent.h"
#include "AngleLimitPointConstraintComponent.h"
#include "AngleLimitHingeConstraintComponent.h"
#include "LimitedBallJointConstraintComponent.h"

#include "DebugSphere.h"
#include "DebugBox.h"
#include "DebugCapsule.h"
#include "EmptyObject.h"
#include "Player.h"
#include "DebugRagdoll.h"

#include "DebugScene.h"

// コンストラクタ
DebugScene::DebugScene()
{
	
}
// 初期化
void DebugScene::Initialize()
{
	InputSystem::LoadAsset("Res/Data/Input/CharacterInput.json");

	std::unique_ptr<Camera> camera{ std::make_unique<Camera>(&worldStorage, objectManager.GenerateNewID()) };
	ResourceManager::LoadCubeTexture("Res/Texture/Sky/NaturalDayMeadow_Cubemap.dds");
	camera->GetComponent<CameraComponent>()->SetSkyTextureHandle(ResourceManager::GetCubeTexture("NaturalDayMeadow_Cubemap.dds"));
	camera->GetComponent<CameraComponent>()->SetClearMode(ClearMode::SKY);

	Camera* cameraPtr{ camera.get()};

	objectManager.Add(std::move(camera));

	std::unique_ptr<Player> player{ std::make_unique<Player>(&worldStorage, objectManager.GenerateNewID(), cameraPtr) };

	objectManager.Add(std::move(player));

	//std::unique_ptr<DebugSphere> sphere{ std::make_unique<DebugSphere>(worldStorage, objectManager.GenerateNewID()) };
	//// sphere->AddComponent<PointConstraintComponent>();
	//objectManager.Add(std::move(sphere));

	std::unique_ptr<DebugBox> debugBox01{ std::make_unique<DebugBox>(&worldStorage, objectManager.GenerateNewID(), 500.0f, 50.0f, 500.0f)};
	debugBox01->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,-200,0 });
	objectManager.Add(std::move(debugBox01));

	// 距離拘束デバッグ(宙ぶらりんなせいで力が減衰する要素がほぼないので凄い動く)
	/*std::unique_ptr<EmptyObject> emptyObject{ std::make_unique<EmptyObject>(&worldStorage, objectManager.GenerateNewID()) };
	emptyObject->GetComponent<TransformComponent>()->SetPosition(Vector3{ 200.0f,0.0f,0.0f });
	DistanceConstraintComponent* distanceConstraint{ emptyObject->AddComponent<DistanceConstraintComponent>(Vector3{ 15.0f,15.0f,15.0f },100.0f) };
	objectManager.Add(std::move(emptyObject));


	std::unique_ptr<DebugBox> debugBox03{ std::make_unique<DebugBox>(&worldStorage, objectManager.GenerateNewID(), 30.0f) };
	debugBox03->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,100,0 });
	debugBox03->AddComponent<RigidBodyComponent>()->SetIsGravity(true);
	distanceConstraint->AddEndPoint(debugBox03->GetID(), Vector3{ 15.0f,15.0f,15.0f });
	objectManager.Add(std::move(debugBox03));*/

	// 点拘束デバッグ
	//std::unique_ptr<DebugBox> debugBox02{ std::make_unique<DebugBox>(&worldStorage, objectManager.GenerateNewID(), 30.0f) };
	//debugBox02->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,50,0 });
	//debugBox02->GetComponent<TransformComponent>()->Rotate(Quaternion::AngleAxis((3.141592f / 4.0f), Vector3{ 0,1,1 }));
	//debugBox02->AddComponent<RigidBodyComponent>()->SetIsGravity(true);
	//PointConstraintComponent* pointConstraint{ debugBox02->AddComponent<PointConstraintComponent>(Vector3{ 15.0f,15.0f,15.0f }) };
	//objectManager.Add(std::move(debugBox02));

	//std::unique_ptr<DebugBox> debugBox04{ std::make_unique<DebugBox>(&worldStorage, objectManager.GenerateNewID(), 30.0f) };
	//debugBox04->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0,100,0 });
	//debugBox04->AddComponent<RigidBodyComponent>()->SetIsGravity(true);
	//pointConstraint->AddEndPoint(debugBox04->GetID(), Vector3{ 15.0f,15.0f,15.0f });
	//objectManager.Add(std::move(debugBox04));

	// 距離拘束によるロープの実装
	/*
			思ったよりええ感じ。
			当り判定は質点の球のみだからコリジョン抜けしまくる
	*/
	/*float distanceRope{ 15.0f };

	Vector3 ropePosition{ -200.0f,0.0f,0.0f };

	std::unique_ptr<EmptyObject> empty1{ std::make_unique<EmptyObject>(worldStorage, objectManager.GenerateNewID()) };
	empty1->GetComponent<TransformComponent>()->SetPosition(ropePosition);
	DistanceConstraintComponent* distanceConstraintRope = empty1->AddComponent<DistanceConstraintComponent>(distanceRope);
	objectManager.Add(std::move(empty1));

	for (int i{ 0 }; i < 10; i++)
	{
		std::unique_ptr<DebugSphere> point{ std::make_unique<DebugSphere>(worldStorage, objectManager.GenerateNewID()) };
		point->GetComponent<SphereColliderComponent>()->SetRadius(2.0f);
		point->GetComponent<TransformComponent>()->SetPosition(ropePosition + Vector3{ distanceRope * i,0.0f,0.0f });
		distanceConstraintRope->AddEndPoint(point->GetID(), Vector3::ZERO);
		distanceConstraintRope = point->AddComponent<DistanceConstraintComponent>(distanceRope);
		objectManager.Add(std::move(point));
	}*/

	// 布のテスト
	/*
			ただ拘束をつなげただけだと柔らかさがない。
			当り判定はロープと同じ
	*/
	//float distanceCloth{ 20.0f };

	//std::vector<DistanceConstraintComponent*> distanceConstraints;
	//std::vector<std::unique_ptr<ObjectBase>> objects;

	//int width{ 8 };
	//int height{ 6};
	//EntityID handle{ objectManager.GenerateNewID() };
	//for (int i{ 0 }; i < height; i++)
	//{
	//	for (int j{ 0 }; j < width; j++)
	//	{
	//		if ((i == 0 && j == 0) || (i == 0 && j == (width - 1)))
	//		{
	//			std::unique_ptr<EmptyObject> point{ std::make_unique<EmptyObject>(&worldStorage, EntityID{handle.GetIndex() + static_cast<uint32_t>(objects.size()),1}) };
	//			point->GetComponent<TransformComponent>()->SetPosition(Vector3{ distanceCloth * j,0.0f,distanceCloth * i });
	//			point->AddComponent<DistanceConstraintComponent>(distanceCloth);
	//			objects.push_back(std::move(point));
	//		}
	//		else
	//		{
	//			std::unique_ptr<DebugSphere> point{ std::make_unique<DebugSphere>(&worldStorage,EntityID{handle.GetIndex() + static_cast<uint32_t>(objects.size()),1}) };
	//			point->GetComponent<SphereColliderComponent>()->SetRadius(2.0f);
	//			point->GetComponent<TransformComponent>()->SetPosition(Vector3{ distanceCloth * j,0.0f,distanceCloth * i });
	//			point->AddComponent<DistanceConstraintComponent>(distanceCloth);
	//			objects.push_back(std::move(point));
	//		}
	//	}
	//}

	//for (auto& obj : objects)
	//{
	//	distanceConstraints.push_back(obj->GetComponent<DistanceConstraintComponent>());
	//}

	//for (int i{ 0 }; i < width * height; i++)
	//{
	//	// 横のつながり
	//	if (i % width != (width - 1))
	//	{
	//		distanceConstraints[i]->AddEndPoint(objects[i + 1]->GetID(), Vector3::ZERO);
	//	}
	//	// 縦のつながり
	//	if (i / width != (height - 1))
	//	{
	//		distanceConstraints[i]->AddEndPoint(objects[i + width]->GetID(), Vector3::ZERO);
	//	}
	//}

	//for (auto& obj : objects)
	//{
	//	objectManager.Add(std::move(obj));
	//}

	/*
			おそらくソフトボディは、成り立たない体積保管がないから。
	*/

	// LoadFile("Res/Data/DebugSceneData.json");

	// ヒンジ拘束
	//std::unique_ptr<EmptyObject> hingeEmpty{ std::make_unique<EmptyObject>(&worldStorage, objectManager.GenerateNewID()) };

	//hingeEmpty->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0.0f,-100.0f,0.0f });

	//HingeConstraintComponent* hinge{ hingeEmpty->AddComponent<HingeConstraintComponent>() };

	//ConstraintTuning angularTuning;

	//angularTuning.stiffness = 100000.0f;
	//angularTuning.damping = 1000.0f;

	//hinge->SetAngularTuning(angularTuning);

	//objectManager.Add(std::move(hingeEmpty));

	//std::unique_ptr<DebugBox> hingeBox{ std::make_unique<DebugBox>(&worldStorage, objectManager.GenerateNewID(), 100.0f, 100.0f, 20.0f) };

	//// 箱の右端がヒンジ位置に一致するように配置
	//hingeBox->GetComponent<TransformComponent>()->SetPosition(Vector3{ -50.0f,-100.0f,0.0f });

	//RigidBodyComponent* hingeBody{ hingeBox->AddComponent<RigidBodyComponent>() };

	//// 固定側と箱側で同じヒンジ軸を指定
	//hinge->AddEndPoint(hingeBox->GetID(), Vector3{ 50.0f,0.0f,0.0f }, Quaternion::Identity());

	//objectManager.Add(std::move(hingeBox));

	// 角度制限付き点拘束
	/*std::unique_ptr<EmptyObject> angleLimitPointEmpty{ std::make_unique<EmptyObject>(&worldStorage, objectManager.GenerateNewID()) };

	angleLimitPointEmpty->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0.0f,-100.0f,0.0f });

	AngleLimitPointConstraintComponent* angleLimitPoint{ angleLimitPointEmpty->AddComponent<AngleLimitPointConstraintComponent>() };

	angleLimitPoint->SetAngleMax(MathConstants::PI_FLT / 1.5f);
	angleLimitPoint->SetAngleMin(MathConstants::PI_FLT / 2.0f);

	objectManager.Add(std::move(angleLimitPointEmpty));

	std::unique_ptr<DebugBox> angleLimitPointBox{ std::make_unique<DebugBox>(&worldStorage, objectManager.GenerateNewID(), 100.0f, 100.0f, 20.0f) };

	angleLimitPointBox->AddComponent<RigidBodyComponent>();

	angleLimitPoint->AddEndPoint(angleLimitPointBox->GetID(), Vector3{ 50.0f,0.0f,0.0f }, Quaternion::Identity());

	objectManager.Add(std::move(angleLimitPointBox));*/

	// 角度制限付きヒンジ拘束
	/*std::unique_ptr<EmptyObject> angleLimitHingeEmpty{ std::make_unique<EmptyObject>(&worldStorage, objectManager.GenerateNewID()) };

	angleLimitHingeEmpty->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0.0f,-100.0f,0.0f });

	AngleLimitHingeConstraintComponent* angleLimitHinge{ angleLimitHingeEmpty->AddComponent<AngleLimitHingeConstraintComponent>() };

	angleLimitHinge->SetAngleMax(MathConstants::PI_FLT / 2.0f);
	angleLimitHinge->SetAngleMin(-MathConstants::PI_FLT / 2.0f);

	objectManager.Add(std::move(angleLimitHingeEmpty));

	std::unique_ptr<DebugBox> angleLimitHingeBox{ std::make_unique<DebugBox>(&worldStorage, objectManager.GenerateNewID(), 100.0f, 100.0f, 20.0f) };

	angleLimitHingeBox->AddComponent<RigidBodyComponent>();

	angleLimitHinge->AddEndPoint(angleLimitHingeBox->GetID(), Vector3{ 50.0f,0.0f,10.0f }, Quaternion::Identity());

	objectManager.Add(std::move(angleLimitHingeBox));*/
	// SwingTwist拘束
	/*std::unique_ptr<EmptyObject> limitedBallJointEmpty{ std::make_unique<EmptyObject>(&worldStorage, objectManager.GenerateNewID()) };

	limitedBallJointEmpty->GetComponent<TransformComponent>()->SetPosition(Vector3{ 0.0f,-100.0f,0.0f });

	LimitedBallJointConstraintComponent* limitedBallJoint{ limitedBallJointEmpty->AddComponent<LimitedBallJointConstraintComponent>() };

	limitedBallJoint->SetSwingAngle(MathConstants::PI_FLT / 3.0f);
	limitedBallJoint->SetTwistAngleMax(MathConstants::PI_FLT / 2.0f);

	objectManager.Add(std::move(limitedBallJointEmpty));

	std::unique_ptr<DebugBox> limitedBallJointBox{ std::make_unique<DebugBox>(&worldStorage, objectManager.GenerateNewID(), 20.0f, 70.0f, 20.0f) };

	limitedBallJointBox->AddComponent<RigidBodyComponent>();

	limitedBallJoint->AddEndPoint(limitedBallJointBox->GetID(), Vector3{ 0.0f,35.0f,0.0f }, Quaternion::IDENTITY);

	objectManager.Add(std::move(limitedBallJointBox));*/

	// Ragdollデバッグ

	objectManager.Add(std::make_unique<DebugRagdoll>(&worldStorage, objectManager.GenerateNewID()));
}
