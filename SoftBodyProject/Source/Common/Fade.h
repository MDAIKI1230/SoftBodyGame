#pragma once

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
	void StartFadeIn();
	void StartFadeOut();
	bool IsFadeOutFinished() const;
	bool IsFadeInFinished() const;
private:
	float alpha{ 0.0f };
	FadeState state{ FadeState::NONE };
};