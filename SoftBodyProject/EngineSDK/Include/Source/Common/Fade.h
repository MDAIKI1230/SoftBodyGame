#pragma once

#include "Color.h"

enum class FadeState
{
	NONE,
	FADE_IN,
	FADE_OUT
};

class Fade
{
public:
	void Update();
	void Draw();
	void SetColor(const Color& _color);
	void SetSpeed(float _speed);
	void StartFadeIn();
	void StartFadeOut();
	bool IsFadeOutFinished() const;
	bool IsFadeInFinished() const;
private:
	Color color{};
	float speed{ 1.0f };
	float alpha{ 0.0f };
	FadeState state{ FadeState::NONE };
};