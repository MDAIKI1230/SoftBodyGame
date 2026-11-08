#include <algorithm>

#include "Renderer.h"
#include "TimeManager.h"
#include "Fade.h"

void Fade::Update()
{
	switch (state)
	{
	case FadeState::NONE:
		return;
	case FadeState::FADE_IN:
		alpha -= TimeManager::GetDeltaTime();
		break;
	case FadeState::FADE_OUT:
		alpha += TimeManager::GetDeltaTime();
		break;
	default:
		break;
	}

	alpha = std::clamp(alpha, 0.0f, 1.0f);

	if (alpha <= 0.0f || alpha >= 1.0f)
	{
		state = FadeState::NONE;
	}
}

void Fade::Draw()
{
	Renderer::DrawFade(alpha);
}

void Fade::StartFadeIn()
{
	state = FadeState::FADE_IN;
	alpha = 1.0f;
}

void Fade::StartFadeOut()
{
	state = FadeState::FADE_OUT;
	alpha = 0.0f;
}

bool Fade::IsFadeOutFinished() const
{
	return alpha >= 1.0f && state == FadeState::NONE;
}

bool Fade::IsFadeInFinished() const
{
	return alpha <= 0.0f && state == FadeState::NONE;
}