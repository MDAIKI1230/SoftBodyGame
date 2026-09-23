#pragma 

#include "RendererComponent.h"

#include "EntityID.h"
#include "AnimationID.h"

struct AnimationComponent
{
	friend class AnimationComponentStorage;
public:
	// コンストラクタ
	AnimationComponent(EntityID _entity, const RendererComponent* _renderer);
	// コンストラクタ
	AnimationComponent(EntityID _entity, const RendererComponent* _renderer, const char* _maskPath);

	// アニメーションの名前の取得
	std::string_view GetAnimationName();
	// アニメーションの名前の変更
	void SetAnimationName(const char* _path);

	// アニメーション再生時間取得
	float GetTime();
	// アニメーション再生時間変更
	void SetTime(float _time);
	// アニメーションスピード取得
	float GetSpeed();
	// アニメーションスピード変更
	void SetSpeed(float _speed);

	// ループ再生フラグ取得
	bool GetLoop();
	// ループ再生フラグ変更
	void SetLoop(bool _isLoop);
	// 再生フラグ取得
	bool GetPlaying();
	// 再生
	void Play();
	// 停止
	void Stop();
private:
	AnimationID id;
};