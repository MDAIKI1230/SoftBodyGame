#include "Renderer.h"

#include "SpriteRenderingSystem.h"

#include "SpriteRendererComponentStorage.h"
#include "TransformComponentStorage.h"

void SpriteRenderingSystem::Draw(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	// レンダラーコンポーネントストレージ
	SpriteRendererComponentStorage* rendererStorage{ static_cast<SpriteRendererComponentStorage*>(_worldStorage->GetStorage<SpriteRendererComponent>()) };
	// Transformストレージ
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };
	// トランスフォーム
	TransformComponent{};
	// 全コンポーネントを描画
	for (EntityID id : rendererStorage->GetEntities())
	{
		const TransformComponent& trans{ transformStorage->Get(id) };

		// レンダー
		const SpriteRendererComponent& renderer{ rendererStorage->Get(id) };

		// 位置取得
		Vector3 position{ trans.GetPosition() };
		// 描画
		Renderer::DrawTexture(renderer.GetHandle(), position);
	}
}