#pragma once

#include "IInput.h"

class DxlibInput:public IInput
{
public:
	// 更新
	void Update();
	// キーが押されたとき
	bool GetKeyPush(KeyConstants keyCode);
	// キーが話されたとき
	bool GetKeyRelease(KeyConstants keyCode);
	// キーが押されている間
	bool GetKeyPress(KeyConstants keyCode);
};