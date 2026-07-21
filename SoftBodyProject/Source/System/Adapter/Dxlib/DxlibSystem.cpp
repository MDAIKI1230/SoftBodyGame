#include <DxLib.h>

#include "DxlibSystem.h"

// 初期化
int DxlibSystem::Init()
{
    const int result = DxLib::DxLib_Init();

    if (result < 0)
    {
        return -1;
    }

    DxLib::SetUseRightHandClippingProcess(TRUE);
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
