#include "ServiceLocator.h"

#include "RendererComponent.h"

#include "TransformComponent.h"

#include "DebugScene.h"

// コンストラクタ
DebugScene::DebugScene()
{

}
// 初期化
void DebugScene::Initialize()
{
	Camera camera{ Vector3{0,0,-100},Vector3{0,0,0} };
	ServiceLocator::GetRenderer()->SetCamera(camera);

	RendererComponent renderer{ ServiceLocator::GetRenderer()->LoadModel(std::string{"Res/Model/M_001_player_073_01.mv1"}) };
	
	GetStorage<RendererComponent>()->Add(0,renderer);

	TransformComponent trans{};

	GetStorage<TransformComponent>()->Add(0, trans);

	state = SceneState::UPDATE;
}
// 終了
void DebugScene::Terminate()
{

}