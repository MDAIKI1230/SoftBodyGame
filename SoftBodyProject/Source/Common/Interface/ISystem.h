#pragma once

class ISystem
{
public:
	// 初期化
	virtual int Init() = 0;
	// ウィンドウモードに変更
	virtual void ChangeWindowMode(bool _flag) = 0;
	/// <summary>
	/// スクリーンサイズとカラーの設定
	/// </summary>
	/// <param name="_screenSizeW">横幅</param>
	/// <param name="_screenSizeH">縦幅</param>
	/// <param name="_colorBit">色</param>
	virtual void SetGraphMode(int _screenSizeW, int _screenSizeH, int _colorBit) = 0;
	// ループ条件
	virtual int ProcessMessage() = 0;
	// 終了
	virtual int End() = 0;
};