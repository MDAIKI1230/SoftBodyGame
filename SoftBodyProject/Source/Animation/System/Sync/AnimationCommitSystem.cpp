#include "ResourceManager.h"

#include "RendererComponent.h"

#include "AnimationCommitSystem.h"

void AnimationCommitSystem::Commit(SkeletonInstanceStorage* _skeletonStorage, WorldStorage* _worldStorage)
{
	// 描画コンポーネントストレージ
	ComponentStorageBase<RendererComponent>* rendererStorage{ _worldStorage->GetStorage<RendererComponent>() };

	for (auto id : _skeletonStorage->GetIDRange())
	{
		// スケルトンストレージからエンティティを取ってきてそれを使って描画コンポーネントを取ってきてそのハンドルを取得する
		ModelHandle model{ rendererStorage->Get(_skeletonStorage->GetOwnerEntity(id)).GetHandle() };

		SkeletonInstanceData& skeleton{ _skeletonStorage->EditSkeletonInstanceData(id) };

		ReBuildMatrices(skeleton.outputPose);
		ResourceManager::ApplyPose(model, skeleton.outputPose);
	}
}

// ポーズの行列を位置/回転/スケールから作り直す
void AnimationCommitSystem::ReBuildMatrices(PoseBuffer& _poseBuffer)
{
	for (int bone{ 0 }; bone < _poseBuffer.localMatrices.size(); bone++)
	{
		_poseBuffer.localRotations[bone].Normalize();

		_poseBuffer.localMatrices[bone] = MatGenerateFunc::TRS(
			_poseBuffer.localPositions[bone],
			_poseBuffer.localRotations[bone],
			_poseBuffer.localScales[bone]);
	}
}
