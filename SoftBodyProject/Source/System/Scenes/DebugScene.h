#pragma once

#include "SceneBase.h"

class DebugScene :public SceneBase
{
public:
	// コンストラクタ
	DebugScene();
	// 初期化
	void Initialize();
	// 終了
	void Terminate();
};