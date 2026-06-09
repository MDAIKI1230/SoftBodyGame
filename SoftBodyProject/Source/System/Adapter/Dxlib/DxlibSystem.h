#pragma once

#include "ISystem.h"

class DxlibSystem:public ISystem
{
public:
	// 初期化
	int Init() override;
	// ウィンドウモードに変更
	void ChangeWindowMode(bool _flag) override;
	/// <summary>
	/// スクリーンサイズとカラーの設定
	/// </summary>
	/// <param name="_screenSizeW">横幅</param>
	/// <param name="_screenSizeH">縦幅</param>
	/// <param name="_colorBit">色</param>
	void SetGraphMode(int _screenSizeW, int _screenSizeH, int _colorBit) override;
	// ループ条件
	int ProcessMessage() override;
	// 終了
	int End() override;
};