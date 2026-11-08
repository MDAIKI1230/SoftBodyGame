#include "ResourceManager.h"

#include "RendererComponent.h"
#include "TransformComponent.h"

#include "AnimationSyncSystem.h"

void AnimationSyncSystem::Sync(SkeletonInstanceStorage* _skeletonStorage, WorldStorage* _worldStorage)
{
	// 描画コンポーネントストレージ
	ComponentStorageBase<RendererComponent>* rendererStorage{ _worldStorage->GetStorage<RendererComponent>() };
	// トランスフォームストレージ
	ComponentStorageBase<TransformComponent>* transformStorage{ _worldStorage->GetStorage<TransformComponent>() };

	for (auto id : _skeletonStorage->GetIDRange())
	{
		EntityID ownerEntity{ _skeletonStorage->GetOwnerEntity(id) };
		// スケルトンストレージからエンティティを取ってきてそれを使って描画コンポーネントを取ってきてそのハンドルを取得する
		ModelHandle model{ rendererStorage->Get(ownerEntity).GetHandle() };

		PoseBuffer& targetPose{ _skeletonStorage->EditTargetPose(id) };
		PoseBuffer& outputPose{ _skeletonStorage->EditOutputPose(id) };

		ResourceManager::GetPose(model, targetPose);
		outputPose = targetPose;

		// 元のTransoform行列を使うことでモデル姿勢をワールド姿勢に変換できるので保持しておく。
		const TransformComponent& trans{ transformStorage->Get(ownerEntity) };
		_skeletonStorage->EditWorldFromModel(id) = trans.GetWorldMatrix();
	}
}
