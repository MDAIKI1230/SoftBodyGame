#include <DxLib.h>

#include "DxlibSystem.h"

// 初期化
int DxlibSystem::Init()
{
	DxLib::SetFullSceneAntiAliasingMode(4, 2);
	DxLib::SetUseDirectInputFlag(true);
	DxLib::SetZBufferBitDepth(32);

    const int result = DxLib::DxLib_Init();

    if (result < 0)
    {
        return -1;
    }

	DxLib::SetUseLighting(TRUE);

	VECTOR lightDirection{
		DxLib::VNorm(DxLib::VGet(0.5f, -1.0f, 0.3f))
	};

	DxLib::ChangeLightTypeDir(lightDirection);
	DxLib::SetLightDifColor(DxLib::GetColorF(1.0f, 0.95f, 0.85f, 1.0f));
	DxLib::SetLightSpcColor(DxLib::GetColorF(0.3f, 0.3f, 0.3f, 1.0f));
	DxLib::SetGlobalAmbientLight(DxLib::GetColorF(0.8f, 0.8f, 0.8f, 1.0f));
	DxLib::SetUseHalfLambertLighting(TRUE);

	DxLib::SetLightAmbColor(
		DxLib::GetColorF(0.25f, 0.27f, 0.3f, 1.0f)
	);

	int fillLight{
		DxLib::CreateDirLightHandle(
			DxLib::VNorm(DxLib::VGet(-0.5f, -0.6f, -0.3f))
		)
	};

	DxLib::SetLightDifColorHandle(
		fillLight,
		DxLib::GetColorF(0.55f, 0.6f, 0.7f, 1.0f)
	);

	DxLib::SetLightAmbColorHandle(
		fillLight,
		DxLib::GetColorF(0.1f, 0.1f, 0.12f, 1.0f)
	);

	DxLib::SetLightSpcColorHandle(
		fillLight,
		DxLib::GetColorF(0.0f, 0.0f, 0.0f, 1.0f)
	);

    return 0;
}

// ウィンドウモードに変更
void DxlibSystem::ChangeWindowMode(bool _flag)
{
	DxLib::ChangeWindowMode(_flag);
}

/// <summary>
/// スクリーンサイズとカラーの設定
/// </summary>
/// <param name="_screenSizeW">横幅</param>
/// <param name="_screenSizeH">縦幅</param>
/// <param name="_colorBit">色</param>
void DxlibSystem::SetGraphMode(int _screenSizeW, int _screenSizeH, int _colorBit)
{
	DxLib::SetGraphMode(_screenSizeW, _screenSizeH, _colorBit);
}

// ループ条件
int DxlibSystem::ProcessMessage()
{
	return DxLib::ProcessMessage();
}
// 終了
int DxlibSystem::End()
{
	return DxLib::DxLib_End();
}
