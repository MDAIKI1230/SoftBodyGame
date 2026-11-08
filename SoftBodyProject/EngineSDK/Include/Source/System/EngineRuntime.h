#pragma once

#include "BackEnd.h"
#include "ResourceOwnerHandle.h"

class EngineRuntime
{
public:
	EngineRuntime(BackEnd&& _backEnd);
	// 初期化成功ならtrue
	bool Initialize();
	// 実行を続けられるならtrue
	bool ProcessMessages();
	// 入力・時間更新、画面クリア
	void BeginFrame();
	// ScreenFlip、フレーム待機
	void EndFrame();
	// 終了処理
	void Shutdown();
private:
	std::unique_ptr<IRenderer> renderer;
	std::unique_ptr<ISystem> system;
	std::unique_ptr<IGPUConnecter> gpuConnecter;
	std::unique_ptr<ISound> sound;
	ResourceOwnerHandle resourceOwnerID{};
};