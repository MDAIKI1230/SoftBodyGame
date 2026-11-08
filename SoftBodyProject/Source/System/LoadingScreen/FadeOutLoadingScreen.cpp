#include "FadeOutLoadingScreen.h"

void FadeOutLoadingScreen::Prepare()
{
	canTerminate = false;
	fade.StartFadeOut();
}

void FadeOutLoadingScreen::Update()
{
	fade.Update();
	canTerminate = fade.IsFadeOutFinished();
}

void FadeOutLoadingScreen::Draw()
{
	fade.Draw();
}