#pragma once

#include "KeyConstants.h"

class IInput
{
public:
	virtual void Update() = 0;
	// キーが押されたとき
	virtual bool GetKeyPush(KeyConstants keyCode) = 0;
	// キーが話されたとき
	virtual bool GetKeyRelease(KeyConstants keyCode) = 0;
	// キーが押されている間
	virtual bool GetKeyPress(KeyConstants keyCode) = 0;
	// デストラクタ
	virtual ~IInput() = default;
protected:
	char pastKeyState[256];
	char currentKeyState[256];
};