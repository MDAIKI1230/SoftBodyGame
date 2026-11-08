#pragma once

#include "IRenderer.h"
#include "IGPUConnecter.h"
#include "ISound.h"

class ServiceLocator
{
	friend class ResourceManager;
	friend class Renderer;
	friend class ApplicationManager;
	friend class SoundPlayer;

private:
	// 取得用関数
	static IRenderer* GetRenderer() { return renderer; }
	static IGPUConnecter* GetGPUConnecter() { return gpuConnecter; }
	static ISound* GetSound() { return sound; }

	// セット用関数
	static void SetRenderer(IRenderer* _renderer) { renderer = _renderer; }
	static void SetGPUConnecter(IGPUConnecter* _gpuConnecter) { gpuConnecter = _gpuConnecter; }
	static void SetSound(ISound* _sound) { sound = _sound; }
private:
	inline static IRenderer* renderer{ nullptr };
	inline static IGPUConnecter* gpuConnecter{ nullptr };
	inline static ISound* sound{ nullptr };
};
