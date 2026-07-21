#include "RaylibInclude.h"

#include "RaylibSystem.h"

// 初期化
int RaylibSystem::Init()
{
	if (!windowMode)
	{
		// InitWindow()より前に設定する必要がある
		::SetConfigFlags(FLAG_FULLSCREEN_MODE);
	}

	::InitWindow(screenSizeW,screenSizeH, windowName.c_str());

	return ::IsWindowReady() ? 0 : -1;
}

// ウィンドウモードに変更
void RaylibSystem::ChangeWindowMode(bool _flag)
{
	windowMode = _flag;

	if (::IsWindowReady())
	{
		const bool currentlyWindowed = !::IsWindowFullscreen();

		if (currentlyWindowed != windowMode)
		{
			::ToggleFullscreen();
		}
	}
}

/// <summary>
/// スクリーンサイズとカラーの設定
/// </summary>
/// <param name="_screenSizeW">横幅</param>
/// <param name="_screenSizeH">縦幅</param>
/// <param name="_colorBit">色</param>
void RaylibSystem::SetGraphMode(int _screenSizeW, int _screenSizeH, int _colorBit)
{
	screenSizeW = _screenSizeW;
	screenSizeH = _screenSizeH;

	// Raylibではバックバッファの色深度をDxLibのSetGraphModeのようには指定しない

	if (::IsWindowReady())
	{
		::SetWindowSize(screenSizeW, screenSizeH);
	}
}

// ループ条件
int RaylibSystem::ProcessMessage()
{
	return ::WindowShouldClose() ? -1 : 0;
}
// 終了
int RaylibSystem::End()
{
	if (::IsWindowReady())
	{
		::CloseWindow();
	}

	return 0;
}
