#pragma once

#include "Fade.h"
#include "LoadingScreen/LoadingScreen.h"

class FadeOutLoadingScreen final : public LoadingScreen
{
public:
	void Prepare() override;
	void Update() override;
	void Draw() override;
private:
	Fade fade;
};