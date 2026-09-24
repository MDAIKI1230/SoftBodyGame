#include "ResourceManager.h"

#include "RigidBodyComponent.h"
#include "BoxColliderComponent.h"
#include "RendererComponent.h"

#include "TutorialBox.h"

TutorialBox::TutorialBox(WorldStorage* _world, EntityID _entityID) :
	ObjectBase{ _world,_entityID }
{
	AddComponent<RigidBodyComponent>()->SetMass(100.0f);
	AddComponent<BoxColliderComponent>(100.0f, 70.0f, 100.0f)->SetOffsetPosition(Vector3{ 0.0f,35.0f,0.0f });
	AddComponent<RendererComponent>(ResourceManager::GetModel("MovableStageRiser.mv1"));

	TransformComponent* trans{ GetComponent<TransformComponent>() };
	trans->SetPosition(Vector3{ 0.0f,30.0f,445.0f });
	trans->ScaleBy(Vector3{ 2.0f,1.0f,2.0f });
}