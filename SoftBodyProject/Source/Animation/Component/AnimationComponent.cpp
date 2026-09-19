#include "AnimationComponentAPI.h"

#include "AnimationComponent.h"

// コンストラクタ
AnimationComponent::AnimationComponent(EntityID _entity, const RendererComponent* _renderer):
	id{ AnimationComponentAPI::CreateAnimation(_entity, _renderer) }
{
}

// コンストラクタ
AnimationComponent::AnimationComponent(EntityID _entity, const RendererComponent* _renderer, const char* _maskPath) :
	id{ AnimationComponentAPI::CreateAnimation(_entity, _renderer,_maskPath) }
{
}

// アニメーションの名前の取得
std::string_view AnimationComponent::GetAnimationName()
{
	return AnimationComponentAPI::GetAnimationName(id);
}
// アニメーションの名前の変更
void AnimationComponent::SetAnimationName(const char* _path)
{
	AnimationComponentAPI::SetAnimationName(id, _path);
}

// アニメーション再生時間取得
float AnimationComponent::GetTime()
{
	return AnimationComponentAPI::GetTime(id);
}
// アニメーション再生時間変更
void AnimationComponent::SetTime(float _time)
{
	AnimationComponentAPI::SetTime(id, _time);
}
// アニメーションスピード取得
float AnimationComponent::GetSpeed()
{
	return AnimationComponentAPI::GetSpeed(id);
}
// アニメーションスピード変更
void AnimationComponent::SetSpeed(float _speed)
{
	AnimationComponentAPI::SetSpeed(id, _speed);
}

// ループ再生フラグ取得
bool AnimationComponent::GetLoop()
{
	return AnimationComponentAPI::GetLoop(id);
}
// ループ再生フラグ変更
void AnimationComponent::SetLoop(bool _isLoop)
{
	AnimationComponentAPI::SetLoop(id, _isLoop);
}
// 再生フラグ取得
bool AnimationComponent::GetPlaying()
{
	return AnimationComponentAPI::GetPlaying(id);
}
// 再生フラグ取得
void AnimationComponent::Play()
{
	AnimationComponentAPI::SetPlaying(id, true);
}
// 再生フラグ変更
void AnimationComponent::Stop()
{
	AnimationComponentAPI::SetPlaying(id, false);
}