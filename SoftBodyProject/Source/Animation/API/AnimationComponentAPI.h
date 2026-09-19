#pragma once

#include "AnimationWorld.h"
#include "RendererComponent.h"

class AnimationComponentAPI
{
public:
	// --- 通常アニメーション ---

	// 生成
	static AnimationID CreateAnimation(EntityID _entity, const RendererComponent* _rendererComponent, const char* _path = nullptr);
	// 破棄
	static void DestroyAnimation(AnimationID _id);

	// アニメーションの名前の取得
	static std::string_view GetAnimationName(AnimationID _id);
	// アニメーションの名前の変更
	static void SetAnimationName(AnimationID _id, const char* _path);

	// アニメーション再生時間取得
	static float GetTime(AnimationID _id);
	// アニメーション再生時間変更
	static void SetTime(AnimationID _id, float _time);
	// アニメーションスピード取得
	static float GetSpeed(AnimationID _id);
	// アニメーションスピード変更
	static void SetSpeed(AnimationID _id, float _speed);

	// ループ再生フラグ取得
	static bool GetLoop(AnimationID _id);
	// ループ再生フラグ変更
	static void SetLoop(AnimationID _id, bool _isLoop);
	// 再生フラグ取得
	static bool GetPlaying(AnimationID _id);
	// 再生フラグ変更
	static void SetPlaying(AnimationID _id, bool _isPlay);

	// --- ラグドール ---

	// 作成
	static RagdollID CreateRagdoll(EntityID _entityID, const RendererComponent& _rendererComponent, const std::string& _path);
	// 破棄
	static void DestroyRagdoll(RagdollID _id);

	// --- アクティブラグドール ---

	// 作成
	static ActiveRagdollID CreateActiveRagdoll(EntityID _entityID, const RendererComponent& _rendererComponent, const std::string& _path);
	// 破棄
	static void DestroyActiveRagdoll(ActiveRagdollID _id);

	static void BindWorld(AnimationWorld* _world);
	static void UnbindWorld();
private:
	AnimationComponentAPI() = default;
private:
	static SkeletonInstanceStorage* skeletonStorage;
	static AnimationStorage* animationStorage;
	static PoseLayerStorage* poseLayerStorage;
	static RagdollStorage* ragdollStorage;
	static ActiveRagdollStorage* activeRagdollStorage;
};
