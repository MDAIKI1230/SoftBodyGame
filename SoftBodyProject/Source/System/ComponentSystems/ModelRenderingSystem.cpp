#include "ServiceLocator.h"

#include "TransformComponent.h"
#include "RendererComponentStorage.h"

#include "ModelRenderingSystem.h"

void ModelRenderingSystem::Draw(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	// レンダラーコンポーネントストレージ
	SparseSetStorageBase<RendererComponent>* rendererStorage{ _worldStorage->GetStorage<RendererComponent>() };
	// Transformストレージ
	SparseSetStorageBase<TransformComponent>* transformStorage{ _worldStorage->GetStorage<TransformComponent>() };
	// エンティティ
	std::vector<EntityID>* entities{ rendererStorage->GetEntities() };
	// トランスフォーム
	TransformComponent trans{};
	// 全コンポーネントを描画
	for (EntityID id : *entities)
	{
		// 取得&チェック
		if (!transformStorage->TryGet(id, trans))
		{
			continue;
		}

		// レンダー
		RendererComponent* renderer{ rendererStorage->Get(id) };
		// 行列をセット
		ServiceLocator::GetRenderer()->ModelSetMatrix(renderer->GetHandle(), trans.GetWorldMatrix());
		// 描画
		ServiceLocator::GetRenderer()->DrawModel(renderer->GetHandle());
	}
}
