#include "ServiceLocator.h"

#include "TransformComponentStorage.h"
#include "RendererComponentStorage.h"

#include "ModelRenderingSystem.h"

void ModelRenderingSystem::Draw(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	// レンダラーコンポーネントストレージ
	RendererComponentStorage* rendererStorage{ static_cast<RendererComponentStorage*>(_worldStorage->GetStorage<RendererComponent>()) };
	// Transformストレージ
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };
	// トランスフォーム
	TransformComponent {};
	// 全コンポーネントを描画
	for (EntityID id : rendererStorage->GetEntities())
	{
		auto trans{ transformStorage->TryGet(id) };
		// 取得&チェック
		if (trans == nullptr)
		{
			continue;
		}

		// レンダー
		auto renderer{ rendererStorage->Get(id) };
		// 行列をセット
		ServiceLocator::GetRenderer()->ModelSetMatrix(renderer.GetHandle(), trans->GetWorldMatrix());
		// 描画
		ServiceLocator::GetRenderer()->DrawModel(renderer.GetHandle());
	}
}
